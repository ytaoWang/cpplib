#include "Thread.h"
#include "MidServer.h"
#include "ThreadPoolConst.h"

Thread::Thread(MidServer *pMidServer):m_pMidServer(pMidServer),m_iStatus(THREAD_SLEEP)
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
  
}

void * Thread::run(void * pThread)
{
  Thread *pThread = static_cast<Thread *>(pThread);
  
  pThread->execute();
}


void Thread::execute()
{
  WorkItem * pWorkItem = m_pMidServer->getTask();
  
}



