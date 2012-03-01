/**
 * @file   Thread.h
 * @author wyt <wyt@wyt-laptop>
 * @date   Thu Mar  1 21:37:50 2012
 * 
 * @brief  Thread executing unit
 * @email:datao0907@163.com
 * 
 */

#ifndef __THREAD_H__
#define __THREAD_H__

#include <pthread.h>

class MidServer;
class Scheduler;

class Thread 
{
 public:
  /** 
   *  
   * @param pMidServer:when this unit need a work,
   * so ask midserver for it 
   */
  Thread(MidServer *pMidServer,bool checked=false,Scheduler *pScheduler);
  /** 
   * pthread execute function
   * 
   * @param pThread :this pointer
   */
  static void* run(void * pThread);
  /** 
   * main for execute,consume product
   * 
   */
  void execute();
  /** 
   * 
   * @return current thread status
   */
  unsigned int getStatus(void) const{
    	return __sync_fetch_and_add(&m_iStatus,0)
      };

  /** 
   * return current thread id
   * 
   * 
   * @return thread id
   */
  pthread_t getTid(void) const
  {
    return m_tid;
  }
  
  /** 
   * 
   * note:this status variable must be synchronous
   * @param status :set current status,mainly for scheduler and itself.
   * 
   */
  void setStatus(unsigned int status);

  ~Thread(void);
  
 private:
  
  //if scheduler don't afford to adjust threadpool size,so do itself.
  void __checked(void);
  
  /// main for get a product and consume then write back
  MidServer* m_pMidServer;
  /// current thread status
  unsigned int m_iStatus;
  /// current thread it
  pthread_t m_tid;
  /// did the scheduler check the thread status?
  bool checked;
  Scheduler *m_pScheduler;
};


#endif
