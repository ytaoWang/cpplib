
/**
 * @file   Scheduler.h
 * @author wyt <wyt@wyt-laptop>
 * @date   Sat Feb 18 22:45:50 2012
 * 
 * @brief  thread scheduler to implement to adjust thread number dynamiclly.
 * @email:datao0907@163.com
 * 
 */

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>
#include <list>
using namespace std::list;

class MidServer;
class Thread;

class Scheduler:public Agent {
 public:
  Scheduler(int32_t min=0,MidServer *pMidServer,int32_t max);
  /** 
   * run this threadpool
   *
   */
  void run(void);
  /** 
   * 
   * these following functions inherit Agent
   *
   * @return fail(-1) or successful(0)
   */
  int sendData(void)
  
  /** 
   * If you have a scheduler,so register it.
   * 
   * 
   * @return fail(-1) or successful(0)
   */
  int recvData(void);
  /** 
   * mainly for threadpool change dynamically
   * 
   * @param value 
   * 
   * @return fail(-1) or successful(0)
   */
  int doAction(unsigned int value);
  /** 
   * 
   *mainly for middleserver if you are based on queue 
   * 
   * @return fail(-1) or successful(0)
   */
  int doAlways(void);
  /** 
   * 
   * 
   * 
   * @return fail(-1) or successful(0)
   */
  int doDestroy(void);
  /** 
   * scheduler destructor 
   * 
   * 
   * @return 
   */
  ~Scheduler(void);
 private:  
  /// min thread number,max thread num ber,actual thread num
  unsigned int min,max,num;
  /// thread list for recording every thread's status 
  list<Thread *> m_mList;
  MidServer *m_pMidServer;
};
#endif
