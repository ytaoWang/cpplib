/**
 * @file   EpollEvent.h
 * @author wyt <wyt@wyt-laptop>
 * @date   Fri Feb  3 22:22:48 2012
 * 
 * @brief  for Epoll Event
 * @email:datao0907@163.com
 * 
 */

/** This class for Epoll event (READ/WRITE/TIMER/AWALY EVENT) management
 * main for open/close corresponding event
 * 
 */
#ifndef __EPOLLEVENT_H__
#define __EPOLLEVENT_H__

class Epoll;
class Agent;

#include <stdint.h>
#include <stdlib.h>

class EpollEvent 
{
  typedef struct 
  {
    uint32_t event;//epoll event
    uint32_t tevent;//always event & timer event
    unsigned int time;//time for  timer event
  } event_st;
  
public:
  /** 
   * EpollEvent construction function(for timer/always agent)
   * 
   * @param pEpoll every EpollEvent has one Epoll
   * @param pAgent every EpollEvent has one corresponding pAgent,but you can set it afterward.
   */
 EpollEvent(Epoll *pEpoll,Agent *pAgent=NULL):m_pEpoll(pEpoll),m_iFd(-1),m_pAgent(pAgent)
  {
    m_eEpollEvent.event = 0;
    m_eEpollEvent.tevent = 0;
    m_eEpollEvent.time = 0;
    m_pAgent->reference();
  }
  /** 
   * EpollEvent construction function(for read/write/timer/always agent)
   * 
   * @param pEpoll every EpollEvent has one Epoll
   * @param fd socket file descriptor
   * @param pAgent 
   */
 EpollEvent(Epoll *pEpoll,int fd,Agent *pAgent=NULL)
 {
   EpollEvent(pEpoll,pAgent);
   m_iFd = fd;
 }
 
  /** 
   * set a new Agent ptr
   * 
   * @param pAgent 
   */
  void setAgent(Agent *pAgent)
  {
    m_eEpollEvent.event = 0;
    m_eEpollEvent.tevent = 0;
    m_eEpollEvent.time = 0;
    
    if(m_pAgent) m_pAgent->release();
    
    m_pAgent = pAgent;
    m_pAgent->reference();
  }
  
  /** 
   * 
   * get corresponding Agent Handler
   *
   * @return agent EpollEvent object belonged to 
   */
  Agent* getAgent() 
  {
    return m_pAgent;
  }
  
  /** 
   * Open Read Event in Epoll,if it is open,so return immediately,modify epoll otherwise,
   * you can call this function many times and don't equate with closeREvent
   *
   * @return fail(-1) or successful(0)
   */
  int openREvent(void);

  /** 
   * Close Read Event in Epoll,if it is close,return immediately,modify epoll otherwise,
   * you can call this function many times and don't equate with openREvent
   *
   * @return fail(-1) or successful(0)
   */
  int closeREvent(void);

  /** 
   * register Read Event in Epoll,when you're interested in read event,so register once,but don't register many times(return fail) and when
   * you're uninterested in read event,so unregister it immediately.when you register it,the read event has opened.
   *
   * @return fail(-1) or successful(0)
   */
  int registerREvent(void);
  
  /** 
   * unregister read event in Epoll,when you're uninterested in read event,so unregister once,same as register times,
   * when you unregister it,write event has closed 
   * 
   * @return fail(-1) or successful(0)
   */
  int unregisterREvent(void);
  
  /** 
   * open write event in Epoll,if it open,so return immediately,modify epoll otherwise,
   * you can call this function many times and don't equate with closeWEvent
   *
   * @return fail(-1) or successful(0) 
   */
  int openWEvent(void);
  
  /** 
   * close write Event in Epoll,if it is close,return immediately,modify epoll otherwise,
   * you can call this function many times and don't equate with openWEvent
   * 
   * 
   * @return fail(-1) or successful(0)
   */
  int closeWEvent(void);
  
  /** 
   * register write event in Epoll,when you're interested in write event,so register once,but don't register many times(return fail) and when 
   * you're uninterested in write event,so unregister it immediately.when you register this event,the write event has written.
   *
   * @return fail(-1) or successful(0)
   * 
   */
  int registerWEvent(void);
  
  /** 
   * unregister read event in Epoll,when you're uninterested in read event,so unregister once,same as register times 
   * 
   * 
   * @return fail(-1) or successful(0)
   */
  int unregisterWEvent(void);
  
  /** 
   * open always event in Epoll,if it open,so return immediately,modify epoll otherwise,
   * you can call this function many times and don't equate with closeAEvent,always event is hidden in epoll,
   * it's also called after all other events if it open,but its premise is that other event must be happened.
   * 
   * @return fail(-1) or successful(0)
   */
  int openAEvent(void);
  
  /** 
   * close always event in Epoll,if it is close,return immediately,modify epoll otherwise,
   * you can call this function many times and don't equate with openAEvent,always event is hidden in epoll,
   * it's also called after all other events if it open,but its premise is that other event must be happened.
   * 
   * @return fail(-1) or successful(0) 
   */
  int closeAEvent(void);
  
  /** 
   * register always event in Epoll,when you're interested in always event,so register once,but don't register many times(return fail) and when 
   * you're uninterested in always event,so unregister it immediately,always event is hidden in epoll,
   * it's also called after all other events if it open,but its premise is that other event must be happened.
   *
   * @return fail(-1) or successful(0)
   * 
   */
  int registerAEvent(void);
  
  /** 
   * 
   * unregister always event in Epoll,when you're uninterested in always event,so unregister once,same as register times 
   * always event is hidden in epoll,it's also called after all other events if it open,but its premise is that other event must be happened.
   * 
   * @return fail(-1) or successful(0)
   * 
   */
  int unregisterAEvent(void);

  /** 
   * open timer event in Epoll ,if it open,so return immediately,modify epoll otherwise,
   * you can call this function many times and don't equate with closeTEvent,timer event is hidden in epoll,
   * it's also called when you set time arrived.
   * 
   * @return fail(-1) or successful(0)
   */
  int openTEvent(void);
  
  /** 
   * close timer event in Epoll,if it is close,return immediately,modify epoll otherwise,
   * you can call this function many times and don't equate with openTEvent,timer event is hidden in epoll,
   * it's also called when you set time arrived.
   * 
   * 
   * @return fail(-1) or successful(0)
   */
  int closeTEvent(void);
  
  /** 
   * modify timer time in Epoll if it is registered,return immediately,modify epoll otherwise,
   * you can call this function many times and don't equate with openTEvent,timer event is hidden in epoll,
   * it's also called when you set time arrived.
   * 
   * @param int the new relative time (ms)
   * 
   * @return fail(-1) or successful(0)
   */
  int modifyTEvent(unsigned int );
  
  /** 
   * register timer event in Epoll,when you're interested in timer event,so register once,but don't register many times(return fail) and when 
   * you're uninterested in timer event,so unregister it immediately,timer event is hidden in epoll.
   * NOTICE,you can register only one timer for one EpollEvent.
   * 
   * @param time:timer argument(ms)
   * @param again:timer properity(only once or again)
   * 
   * @return fail(-1) or successful(0) 
   */
  int registerTEvent(unsigned int time,bool again=true);
  
  /** 
   * unregister timer event in Epoll,when you're uninterested in timer event,so unregister once,same as register times 
   * timer event is hidden in epoll,it's also called but its premise is that its time is arrived.
   * 
   * 
   * @return fail(-1) or successful(0) 
   */
  int unregisterTEvent(void);
  
  int getEventfd() const 
  {
    return m_iFd;
  }
  
  /** 
   * EpollEvent destruct function
   * 
   */
  ~EpollEvent()
  {

  }
  
  
private:
   /// EpollEvent belongs to the Epoll
  Epoll *m_pEpoll;
  /// EpollEvent belongs to one Agent
  Agent *m_pAgent;
  /// EpollEvent interested event
  event_st m_eEpollEvent;
  /// EpollEvent monitor socket/fd
  int m_iFd;
};

#endif  
