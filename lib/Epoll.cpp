#include "Epoll.h"
#include "EpollEvent.h"
#include "event.h"
#include "Error.h"
#include "Agent.h"

#include <sys/time.h>
#include <string.h>

Epoll::Epoll(unsigned int size):m_iEventSize(size),m_pEpollEvent(NULL)
{
  if((m_iFd = epoll_create(10))<0)
  {
    handleError("Epoll::Epoll");
  }
  
  m_pEpollEvent = new struct epoll_event[m_iEventSize];
}

Epoll::~Epoll(void)
{
  event_t *pEventHandler;
  if(m_pEpollEvent)
    delete []m_pEpollEvent;
  for(map<EpollEvent *,event_t *>::iterator it = m_mAlways.begin();it != m_mAlways.end();)
  {   
    pEventHandler = it->second;
    m_mAlways.erase(it++);
    --pEventHandler->ref;
    checkRef(pEventHandler->pEpollEvent);
  }

  for(multimap<uint64_t,event_t *>::iterator it = m_mTimer.begin();it != m_mTimer.end();)
  {
    pEventHandler = it->second;
    m_mTimer.erase(it++);
    --pEventHandler->ref;
    checkRef(pEventHandler->pEpollEvent);
  }
  
  for(map<EpollEvent*,event_t *>::iterator it = m_mList.begin();it != m_mList.end();)
  {
    pEventHandler = it->second;
    m_mList.erase(it++);
    
    Agent *pAgent = pEventHandler->pEpollEvent->getAgent();
    if(pAgent) pAgent->release();
   
    delete pEventHandler;
  }
}

int Epoll::doEvent(EpollEvent *pEpollEvent,int op,uint32_t event)
{
  int fd = pEpollEvent->getEventfd();
  struct epoll_event ev;
  
  memset(&ev,0,sizeof(ev));
  ev.events = event;
  ev.data.ptr = pEpollEvent;
  
  if(epoll_ctl(m_iFd,op,fd,&ev)<0)
  {
    handleSyscallError("Epoll::doEvent");
    return -1;
  }
  
  return 0;
}

int Epoll::doOEvent(EpollEvent *pEpollEvent,int op,uint32_t event,int time)
{
  switch(op)
  {
  case EVENT_DEL:
  case EVENT_ADD:
    {
      switch(event)
      {
      case EVENT_TIMER_AGAIN:
      case EVENT_TIMER_ONCE:
        return timerCtl(pEpollEvent,op,event,time);
      case EVENT_ALWAYS:
        return alwaysCtl(pEpollEvent,op,event);
      default:handleError("Epoll::doOEvent");
        break;
      }
      
    } break;
  default:break;
  }

  if(event & ~EVENT_CHANGE) return -1;
  
  map<EpollEvent *,event_t *>::iterator it; 
  event_t *pEventHandler;

  it = m_mList.find(pEpollEvent);
  if(it == m_mList.end()) return -1;

  pEventHandler = it->second;
  pEventHandler->tevent = event;
  
  return 0;
}

int Epoll::timerCtl(EpollEvent *pEpollEvent,int op,uint32_t event,int time)
{
  //check event valid
  if(event & (EVENT_TIMER_AGAIN | EVENT_TIMER_ONCE)) return -1;
  
  event_t *pEventHandler;
  map<EpollEvent *,event_t *>::iterator it;
  
  pEventHandler = NULL;
  
  switch(op)
  {
  case EVENT_ADD:
    {
      if(time <=0) return -1;
      //check register valid
      it = m_mList.find(pEpollEvent);
      //may be first for always event
      if(it != m_mList.end()) 
      {  
        pEventHandler = it->second;
        //register only once
        if(pEventHandler->tevent & (EVENT_TIMER_AGAIN | EVENT_TIMER_ONCE)) return -1;
        pEventHandler->tevent |= event;
      }
      else
      {
      pEventHandler = new event_t;
      memset(pEventHandler,0,sizeof(*pEventHandler));
      
      pEventHandler->ref = 1;
      pEventHandler->time = time;
      pEventHandler->tevent = event;
      pEventHandler->pEpollEvent = pEpollEvent;
      
      if(!m_mList.insert(pair<EpollEvent *,event_t *>(pEpollEvent,pEventHandler)).second)
      {
        delete pEventHandler;
        return -1;
      }

      Agent *pAgent = pEpollEvent->getAgent();
      pAgent->reference();

      }
      uint64_t t;
      if((t=getCurrentTime())<0)
      {
        delete pEventHandler;
        return -1;
      }
      
      time = t + time;
      pair<multimap<uint64_t,event_t *>::iterator,multimap<uint4_t,event_t *>::iterator> ret;
      ret = m_mTimer.equal_range(time);
      
      for(map<uint64_t,event_t *>::iterator iter = ret.first;iter != ret.second;++iter)
      {
        if(iter->second == pEventHandler) return -1;
      }

      m_mTimer.insert(pair<uint64_t,event_t *>(time,pEventHandler));
      ++pEventHandler->ref;
    }
    break;
  case EVENT_DEL:
    {
      //check register valid
      it = m_mList.find(pEpollEvent);
      if(it == m_mList.end()) return 0;
      pEventHandler = it->second;
      //don't erase from m_mTimer now,just clear its timer flag,until its time,then check flag
      pEventHandler->tevent |= EVENT_TIMER_DEL;
      /*
      //don't be refered any more
      if(--pEventHandler->ref <=1 )
      {
        m_mList.erase(pEpollEvent);
        delete pEventHandler;
      }
      */
    }
    break;
  default:break;
  }
  
  return 0;
}

int Epoll::alwaysCtl(EpollEvent *pEpollEvent,int op,uint32_t event)
{
  if(event != EVENT_ALWAYS) return -1;
  
  map<EpollEvent *,event_t *>::iterator it;
  event_t *pEventHandler;
  
  switch(op)
  {
  case EVENT_ADD:
    {
      it = m_mList.find(pEpollEvent);
      if(it != m_mList.end())
      {
        pEventHandler = it->second;
        //register only once
        if(pEventHandler->tevent & EVENT_ALWAYS) return -1;
        pEventHandler->tevent |= event;
      }
      else
      {
        pEventHandler = new event_t;
        memset(pEventHandler,0,sizeof(*pEventHandler));
        
        pEventHandler->ref = 1;
        pEventHandler->time = 0;
        pEventHandler->tevent = event;
        pEventHandler->pEpollEvent = pEpollEvent;
        
        if(!m_mList.insert(pair<EpollEvent *,event_t *>(pEpollEvent,pEventHandler)).second)
        {
          delete pEventHandler;
          return -1;
        }
        
        Agent *pAgent = pEpollEvent->getAgent();
        pAgent->reference();
        
      }
      
      if(!m_mAlways.insert(pair<EpollEvent *,event_t *>(pEpollEvent,pEventHandler)).second)
      { 
        delete pEventHandler;
        return -1;
      }
      
      ++pEventHandler->ref;
    }
    break;
  case EVENT_DEL:
    {
      it = m_mList.find(pEpollEvent);
      if(it == m_mList.end()) return 0;
      pEventHandler = it->second;
      //don't erase from m_mTimer now,just clear its timer flag,until its event arrived,then check flag
      pEventHandler->tevent |= EVENT_ALWAYS_DEL;
    }
    break;
  }
  
  return 0;
}

uint64_t Epoll::getCurrentTime()
{
  struct timeval tv;
  if(gettimeofday(&tv,NULL)<0) return -1;
  
  return tv.tv_sec*1000+tv.tv_usec;
}

void Epoll::checkRef(EpollEvent *pEpollEvent)
{
  map<EpollEvent *,event_t *>::iterator it = m_mList.find(pEpollEvent);
  event_t *pEventHandler;
  if(it != m_mList.end())
  {
    pEventHandler = it->second;

    if(1 == pEventHandler->ref) 
    {
      m_mList.erase(pEventHandler->pEpollEvent);
      Agent *pAgent = pEventHandler->pEpollEvent->getAgent();
      if(pAgent) pAgent->doDestroy();
      
      delete pEventHandler;
    }
  }
}

void Epoll::checkTimer(void)
{
  event_t * pEventHandler;
  
  uint64_t value;
  if((value = getCurrentTime())<0) return;

  map<uint64_t,event_t *>::iterator it ;//= m_mTimer.begin();
  //if(it == m_mTimer.end()) return;
  
   //timer is triggered
  do
  {
    it = m_mTimer.begin();
    if(it == m_mTimer.end()) break;
    pair<multimap<uint64_t,event_t *>::iterator,multimap<uint64_t,event_t *>::iterator> ret;
    ret = m_mTimer.equal_range(it->first);
    for(map<uint64_t,event_t *>::iterator iter = ret.first;iter != ret.second;)
    {
      //check flag
      pEventHandler = iter->second;
      if(pEventHandler->tevent & EVENT_TIMER_DEL)
      {
        m_mTimer.erase(iter++);
        --pEventHandler->ref;
        checkRef(pEventHandler->pEpollEvent);
        continue;
      }
      //doAction function is called
      if(pEventHandler->tevent & EVENT_TIMER_AGAIN)
      {
        Agent *pAgent = pEventHandler->pEpollEvent->getAgent();
        if(pAgent->doAction(pEventHandler->time)<0) pEventHandler->tevent |= EVENT_TIMER_DEL;
        ++iter;
        continue;
      }
      if(pEventHandler->tevent & EVENT_TIMER_ONCE)
      {
        Agent *pAgent = pEventHandler->pEpollEvent->getAgent();
        pAgent->doAction(pEventHandler->time);
        pEventHandler->tevent |= EVENT_TIMER_DEL;
        continue;
      }
    }
  } while(value <= it->first);
  
  
}

void Epoll::checkAlways(void)
{
  event_t *pEventHandler;
  
  for(map<EpollEvent *,event_t *>::iterator it = m_mAlways.begin();it != m_mAlways.end();)
  {
    pEventHandler = it->second;
    if(pEventHandler->tevent & EVENT_TIMER_DEL)
    {
      m_mAlways.erase(it++);
      --pEventHandler->ref;
      checkRef(pEventHandler->pEpollEvent);
      continue;
    }
    //doAlways function is called here
    if(pEventHandler->tevent & EVENT_ALWAYS)
    {
      Agent *pAgent = pEventHandler->pEpollEvent->getAgent();
      if(pAgent->doAlways() < 0) pEventHandler->tevent |= EVENT_ALWAYS_DEL;
      ++it;
      
      continue;
    }
    
  }
}

void Epoll::run(void)
{
  int timeout = -1;
  int size;
  EpollEvent *pEpollEvent;
  
  //get mini wait time from timer
  map<uint64_t,event_t *>::iterator it = m_mTimer.begin();
  if(it != m_mTimer.end())
    timeout = it->second->time;  

  size = epoll_wait(m_iFd,m_pEpollEvent,m_iEventSize,timeout);
  //timer event check
  checkTimer();
  //epoll read/write event check
  for(int i = 0;i < size;++i)
  {
    pEpollEvent = static_cast<EpollEvent *>(m_pEpollEvent[i].data.ptr);
    if(!pEpollEvent) continue;
    
    Agent *pAgent = pEpollEvent->getAgent();
    if(!pAgent) continue;

    //ready for reading
    if(m_pEpollEvent[i].events & EPOLLIN) 
    {
      if(pAgent->recvData()<0)
      {
        //don't own other event
        if(m_mList.find(pEpollEvent) == m_mList.end())
          pAgent->doDestroy();
        continue;
      }
    }
    //ready for writing
    if(m_pEpollEvent[i].events & EPOLLOUT)
    {
      if(pAgent->sendData()<0)
      {
        //don't own other event
        if(m_mList.find(pEpollEvent) == m_mList.end())
          pAgent->doDestroy();
        continue;
      }
    }
    if(m_pEpollEvent[i].events & (EPOLLERR | EPOLLHUP))
    {
      if(m_mList.find(pEpollEvent) == m_mList.end())
        pAgent->doDestroy();
      continue;
    }
  }
  
  //always event check
  checkAlways();
}
