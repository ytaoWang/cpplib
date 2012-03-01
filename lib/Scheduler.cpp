#include "Scheduler.h"

#include <unistd.h>

Scheduler::Scheduler(uint32_t _min,MidServer *pMidServer,int32_t _max):min(_min),max(_max),m_pMidServer(pMidServer)
{
  int num = sysconf(_SC_NRPROCESSORS_ONLN);
  num = static_cast<int>(num * 1.5);
  max = num > max?max:num;
  num = (min + max)/2;
}

int Scheduler::sendData(void) 
{
  return 0;
}

void Scheduler::run(void)
{
  for(int i = 0;i < num;++i)
  {
    
  }
}

int Scheduler::recvData(void)
{
  return 0;
}

int Scheduler::doAction(unsigned int value)
{
  return 0;
}

int Scheduler::doAlways(void)
{
  return 0;
}

int Scheduler::doDestroy(void)
{
  return 0;
}
