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

class Thread 
{
 public:
  /** 
   *  
   * @param pMidServer:when this unit need a work,
   * so ask midserver for it 
   */
  Thread(MidServer *pMidServer);
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
  unsigned int getStatus(void) const{return m_iStatus};
  
  /** 
   * 
   * note:this status variable must be synchronous
   * @param status :set current status,mainly for scheduler and itself.
   * 
   */
  void setStatus(unsigned int status);
  
 private:
  /// main for get a product and consume then write back
  MidServer* m_pMidServer;
  /// current thread status
  unsigned int m_iStatus;
  pthread_t m_tid;
  
};


#endif
