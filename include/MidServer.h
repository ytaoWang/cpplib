/**
 * @file   MidServer.h
 * @author wyt <wyt@wyt-laptop>
 * @date   Sat Feb 18 23:05:17 2012
 * 
 * @brief  MidServer class make thread get task
 *         and then report its result to MidServer,
 *         please note that this class is base class.
 * @email:datao0907@163.com
 * 
 */

#ifndef __MIDSERVER_H__
#define __MIDSERVER_H__

class WokItem;
class MidServer {
 public:
  /** 
   * this function is for thread to get a Task
   * @return task
   */
  virtual WorkItem* getTask()=0;
  /** 
   * when upper thread want threadpool to execute a task,it calls 
   * this function to add task
   * 
   * @param task the unit for thread to execute
   * 
   * @return fail(-1) or successful(0)
   */
  virtual int setTask(WorkItem * task)=0;
  /** 
   * 
   * when the MidServer is imbalance or other,then you can call this to
   * adjust it,execute per one second
   * 
   * @return 
   */
  virtual int sync() = 0;
  /** 
   * when the scheduler wants to know accurate number of 
   * current task queue,the following function is called.
   * 
   * 
   * @return the number of current task queue.
   */
  virutal int getTaskNum() = 0;

};



#endif
