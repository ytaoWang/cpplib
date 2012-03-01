#include "EpollEvent.h"
#include "Epoll.h"
#include "event.h"
#include "Error.h"

#include <errno.h>

int EpollEvent::openREvent(void)
{
  if(m_eEpollEvent.event & EVENT_READ) return 0;
  
  m_eEpollEvent.event |= EVENT_READ;
  
  if(-1 == m_pEpoll->doEvent(this,EVENT_CHANGE,m_eEpollEvent.event))
  {
    handleError("EpollEvent::openREvent");
    return -1;
  }

  return 0;
}

int EpollEvent::closeREvent(void)
{
  if(m_eEpollEvent.event & ~EVENT_READ) return 0;
  
  m_eEpollEvent.event &= ~EVENT_READ;
  
  if(-1 == m_pEpoll->doEvent(this,EVENT_CHANGE,m_eEpollEvent.event))
  {
    handleError("EpollEvent::closeREvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::registerREvent(void)
{
  if(m_eEpollEvent.event & EVENT_READ) 
  {
    errno = EEXIST;
    return -1;
  }
  
  m_eEpollEvent.event |= EVENT_READ;
  
  if(-1 == m_pEpoll->doEvent(this,EVENT_ADD,EVENT_READ))
  {
    handleError("EpollEvent::registerREvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::unregisterREvent(void)
{
  if(m_eEpollEvent.event & ~EVENT_READ)
    return 0;
  
  m_eEpollEvent.event &= ~EVENT_READ;
  
  if(-1 == m_pEpoll->doEvent(this,EVENT_DEL,EVENT_READ))
  {
    handleError("EpollEvent::unregisterREvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::registerWEvent(void)
{
  if(m_eEpollEvent.event & EVENT_WRITE)
  {
    errno = EEXIST;
    return -1;
  }
  
  m_eEpollEvent.event |= EVENT_WRITE;

  if(-1 == m_pEpoll->doEvent(this,EVENT_ADD,EVENT_WRITE))
  {
    handleError("EpollEvent::registerWEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::unregisterWEvent(void)
{
  if(m_eEpollEvent.event & ~EVENT_WRITE)
    return 0;
  
  if(-1 == m_pEpoll->doEvent(this,EVENT_DEL,EVENT_WRITE))
  {
    handleError("EpollEvent::unregisterWEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::openWEvent(void)
{
  if(m_eEpollEvent.event & EVENT_WRITE) return 0;
  
  m_eEpollEvent.event |= EVENT_WRITE;
  
  if(-1 == m_pEpoll->doEvent(this,EVENT_CHANGE,m_eEpollEvent.event))
  {
    handleError("EpollEvent::openWEvent");
    return -1;
  }
  return 0;
}

int EpollEvent::closeWEvent(void)
{
  if(m_eEpollEvent.event & ~EVENT_WRITE) return 0;
  
  m_eEpollEvent.event &= ~EVENT_WRITE;
  
  if(-1 == m_pEpoll->doEvent(this,EVENT_CHANGE,m_eEpollEvent.event))
  {
    handleError("EpollEvent::closeWEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::registerAEvent(void)
{
  if(m_eEpollEvent.tevent & EVENT_ALWAYS)
  {
    errno = EEXIST;
    return -1;
  }
  
  m_eEpollEvent.tevent |= EVENT_ALWAYS;
  
  if(-1 == m_pEpoll->doOEvent(this,EVENT_ADD,EVENT_ALWAYS))
  {
    handleError("EpollEvent::registerAEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::unregisterAEvent(void)
{
  if(m_eEpollEvent.tevent & ~EVENT_ALWAYS)
    return 0;
  
  m_eEpollEvent.tevent &= ~EVENT_ALWAYS;
  
  if(-1 == m_pEpoll->doOEvent(this,EVENT_DEL,EVENT_ALWAYS))
  {
    handleError("EpollEvent::unregisterAEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::openAEvent(void)
{
  if(m_eEpollEvent.tevent & EVENT_ALWAYS) return 0;
  
  m_eEpollEvent.tevent |= EVENT_ALWAYS;
  
  if(-1 == m_pEpoll->doOEvent(this,EVENT_CHANGE,m_eEpollEvent.tevent))
  {
    handleError("EpollEvent::openAEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::closeAEvent(void)
{
  if(m_eEpollEvent.tevent & ~EVENT_ALWAYS) return 0;
  
  m_eEpollEvent.tevent &= ~EVENT_ALWAYS;
  
  if(-1 == m_pEpoll->doOEvent(this,EVENT_CHANGE,m_eEpollEvent.tevent))
  {
    handleError("EpollEvent::closeAEvent");
    return -1;
  }
  return 0;
}

int EpollEvent::registerTEvent(unsigned int time,bool again)
{
  unsigned int tevent = 0;
  
  if(m_eEpollEvent.tevent & (EVENT_TIMER_ONCE|EVENT_TIMER_AGAIN))
    return 0;
  
  if(again)
    tevent = EVENT_TIMER_AGAIN;
  else
    tevent = EVENT_TIMER_ONCE;
  
  
  m_eEpollEvent.tevent |= tevent;
  m_eEpollEvent.time = time;
  
  
  if(-1 == m_pEpoll->doOEvent(this,EVENT_ADD,tevent,time))
  {
    handleError("EpollEvent::registerTEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::unregisterTEvent(void)
{
  unsigned int tevent;
  
  if(m_eEpollEvent.tevent & ~(EVENT_TIMER_ONCE | EVENT_TIMER_AGAIN))
    return 0;
  
  if(m_eEpollEvent.tevent & EVENT_TIMER_ONCE) 
    tevent = EVENT_TIMER_ONCE;
  else
    tevent = EVENT_TIMER_AGAIN;

  m_eEpollEvent.tevent &= ~(EVENT_TIMER_ONCE | EVENT_TIMER_AGAIN);

  if(-1 == m_pEpoll->doOEvent(this,EVENT_DEL,tevent))
  {
    handleError("EpollEvent::unregisterTEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::openTEvent(void)
{
  unsigned int tevent;
  
  if(m_eEpollEvent.tevent & (EVENT_TIMER_AGAIN | EVENT_TIMER_ONCE))
    return 0;
  if(m_eEpollEvent.tevent & EVENT_TIMER_AGAIN)
    tevent = EVENT_TIMER_AGAIN;
  else 
    tevent = EVENT_TIMER_ONCE;
  
  m_eEpollEvent.tevent |= tevent;

  if(-1 == m_pEpoll->doOEvent(this,EVENT_CHANGE,m_eEpollEvent.tevent))
  {
    handleError("EpollEvent::openTEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::closeTEvent(void)
{
  
  if(m_eEpollEvent.tevent & ~(EVENT_TIMER_AGAIN | EVENT_TIMER_ONCE))
    return 0;

  if(m_eEpollEvent.tevent & EVENT_TIMER_AGAIN)
    m_eEpollEvent.tevent &= ~EVENT_TIMER_AGAIN;
  else 
    m_eEpollEvent.tevent &= ~EVENT_TIMER_ONCE;

  if(-1 == m_pEpoll->doOEvent(this,EVENT_CHANGE,\
                                   m_eEpollEvent.tevent))
  {
    handleError("EpollEvent::openTEvent");
    return -1;
  }
  
  return 0;
}

int EpollEvent::modifyTEvent(unsigned int value)
{
  unsigned int tevent;
  
  if(m_eEpollEvent.tevent & ~(EVENT_TIMER_AGAIN | EVENT_TIMER_ONCE))
    return 0;
  
  if(m_eEpollEvent.tevent & EVENT_TIMER_AGAIN)
    tevent = EVENT_TIMER_AGAIN;
  else 
    tevent = EVENT_TIMER_ONCE;
  
  if(-1 == m_pEpoll->doOEvent(this,EVENT_DEL,tevent))
  {
    handleError("EpollEvent::modifyTEvent");
    return -1;
  }
  
  m_eEpollEvent.time = value;
  
  if(-1 == m_pEpoll->doOEvent(this,EVENT_ADD,tevent,value))
  {
    handleError("EpollEvent::modifyTEvent");
    return -1;
  }
  
  return 0;
}

