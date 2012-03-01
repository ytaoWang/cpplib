/**
 * @file   Epoll.h
 * @author wyt <wyt@wyt-laptop>
 * @date   Sat Feb  4 23:58:12 2012
 * 
 * @brief  Epoll operation class
 * @email:datao0907@163.com
 * 
 */

#ifndef __EPOLL_H__
#define __EPOLL_H__

#include <stdint.h>
#include <stdlib.h>
#include <map>
#include <set>
using namespace std;

/**
 * the epoll class is used for main loop in a thread,\
 * which drive all other member/event
 * 
 */

class EpollEvent;

class Epoll
{
  /// data structure for timer
  /*
  typedef struct
  {
    EpollEvent *pEpollEvent;//event handler
    uint32_t tevent;//timer event
    unsigned int time;
  }timer_t;
  */
  /// data structure for timer/always event
  typedef struct 
  {
    EpollEvent *pEpollEvent; //event handler
    uint32_t tevent;    //event type for always
    unsigned int time;  //time(relative value)
    int ref;           //reference count
  }event_t;
  
  
 public:
  /** 
   * initialize epoll and other member 
   * 
   * @param size listen fd size
   */
  Epoll(unsigned int size);
  ~Epoll(void);
  
  /** 
   * operation(ADD/MOD/DEL) for read/write event,argument for epoll_ctl system call
   * 
   * @param pEpollEvent 
   * @param op EVENT_ADD/EVENT_CHANGE/EVENT_DEL
   * @param event EVENT_READ/EVENT_WRITE
   * 
   * @return 
   */
  int doEvent(EpollEvent *pEpollEvent,int op,uint32_t event);
  
  /** 
   * operation(ADD/MOD/DEL) for always event
   * 
   * @param pEpollEvent 
   * @param op EVENT_ADD/EVENT_CHANGE/EVENT_DEL
   * @param event EVENT_ALWAYS
   * 
   * @return fail(-1) or successful(0)
   */
  //  int doOEvent(EpollEvent *pEpollEvent,int op,uint32_t event);
  /** 
   * operation(ADD/MOD/DEL) for timer event
   * 
   * @param pEpollEvent 
   * @param op EVENT_ADD/EVENT_CHANGE/EVENT_DEL
   * @param event EVENT_TIMER_ONCE/EVENT_TIMER_AGAIN
   * @param time 
   * 
   * @return 
   */
  int doOEvent(EpollEvent *pEpollEvent,int op,uint32_t event,int time=0);
  /** 
   * main loop function,drive all other member 
   * 
   * 
   */
  void run(void);
 private:
  
  /** 
   * timer operation(ADD/DEL)
   * 
   * @param pEpollEvent 
   * @param op 
   * @param event 
   * @param time 
   * 
   * @return fail(-1) or successful(0)
   */
  int timerCtl(EpollEvent *pEpollEvent,int op,uint32_t event,int time);
  /** 
   * always operation(ADD/DEL)
   * 
   * @param pEpollEvent 
   * @param op 
   * @param event 
   * 
   * @return fail(-1) or successful(0)
   */
  int alwaysCtl(EpollEvent *pEpollEvent,int op,uint32_t event);

  /** 
   * get current time(ms)
   * 
   * 
   * @return ms
   */
  uint64_t getCurrentTime();
  
  /** 
   * if event_t's reference is 1,then delete it.
   * 
   */
  void checkRef(EpollEvent *pEpollEvent);
  
  /** 
   * Timer EVENT check
   * 
   */
  void checkTimer();
  /** 
   * Always EVENT check
   * 
   */
  void checkAlways();
  
  /// Epoll fd
  int m_iFd;
  /// save always event handler
  map<EpollEvent *,event_t *> m_mAlways;
  /// save timer event handler,the key is time(absolute value),difference value has same key and same value has different key
  multimap<uint64_t,event_t *> m_mTimer;
  /// save all live EpollEvent <==> event_t
  map<EpollEvent *,event_t *> m_mList;
  struct epoll_event *m_pEpollEvent;
  int m_iEventSize;
};

#endif
