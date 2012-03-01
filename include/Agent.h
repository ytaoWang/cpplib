/**
 * @file   Agent.h
 * @author wyt <wyt@wyt-laptop>
 * @date   Sun Feb  5 23:31:26 2012
 * 
 * @brief  base Agent class
 * @email:datao0907@163.com
 * 
 */

#ifndef __AGENT_H__
#define __AGENT_H__

#include "EpollEvent.h"

class Epoll;

class Agent
{
 public:
  Agent(Epoll *pEpoll)
  {
    m_pEpollEvent = new EpollEvent(pEpoll,this);
  }
  
  /** 
   * when epoll hints that there has some data ready for writing
   * 
   * 
   * @return fail(-1) or successful(0)
   */
  virtual int sendData(void) = 0;
  /** 
   * when epoll hints that there has some data ready for reading
   * 
   * 
   * @return fail(-1) or successful(0)
   */
  virtual int recvData(void) = 0;
  /** 
   * when epoll hints that the time setted has arrived.
   * 
   * @param time set timer value when intialized.
   * 
   * @return fail(-1) or successful(0)
   */
  virtual int doAction(unsigned int time) = 0;
  /** 
   * when epoll hints that always event has arrived
   * 
   * 
   * @return fail(-1) or successful(0)
   */
  virtual int doAlways(void) = 0;
  /** 
   * epoll will call this function  when this agent's reference count is 0 
   * before deletion.
   * 
   * 
   * @return fail(-1) or successful(0)
   */
  virtual int doDestroy(void) = 0;
  /** 
   * when this class is refered by other object,will add its reference
   * 
   */
  void reference(void) 
  {
    __sync_fetch_and_add(&ref,1);
  }
  /** 
   * when this object is release by other object,the following function will be called.x
   * 
   */
  void release(void)
  {
    if(0 == __sync_sub_and_fetch(&ref,1))
      this->doDestroy();
  }
  `~Agent(void)
  {
    delete m_pEpollEvent;
  }
  
 protected:
  /// Agent reference count
  int32_t ref;
  /// EpollEvent own to Agent
  EpollEvent *m_pEpollEvent;
  
};


#endif
