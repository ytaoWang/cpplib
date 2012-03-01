#include "Thread.h"
#include "MidServer.h"
#include "ThreadPoolConst.h"
#include "Scheduler.h"

Thread::Thread(MidServer *pMidServer,bool _checked,Scheduler *pScheduler):m_pMidServer(pMidServer),m_iStatus(THREAD_SLEEP),checked(_checked),m_pScheduler(pScheduler)
{
  if(pthread_create(&m_tid,NULL,run,this)) 
  {
    handleSyscallError("create thread error");
    return;
  }
  
  if(pthread_detach(m_tid)) 
  {
    handleSyscallError("detach thread error");
    return;
  }
  
  m_pScheduler->reference();
}

void * Thread::run(void * pThread)
{
  Thread *pThread = static_cast<Thread *>(pThread);
  
  pThread->execute();
}

Thread::~Thread()
{
  m_pScheduler->release();
}


void Thread::execute()
{
  WorkItem * pWorkItem;
  int oldstate;
  
  while(1) 
  {
    setStatus(THREAD_SLEEP);
    pWorkItem = m_pMidServer->getTask();
    //set work status
    setStatus(THREAD_RUNNING);
    //set cancel status disable
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,&oldstate);
    //begin to work
    //pWorkItem->work();
    //set cancel status enable
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,&oldstate);
    __checked();
  }
}

void Thread::__checked(void)
{
  if(!checked) return;
  
  //get the number of task num
  //decide how to make choice,if
}

void Thread::setStatus(unsigned int status)
{
  __sync_lock_test_and_set(&m_iStatus,status);
}



