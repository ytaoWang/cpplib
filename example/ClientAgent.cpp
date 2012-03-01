#include "ClientAgent.h"
#include "EpollEvent.h"

#include <iostream>

using namespace std;

ClientAgent::ClientAgent(Epoll *pEpoll)
{
  m_pEpollEvent = new EpollEvent(pEpoll,this);
  if(m_pEpollEvent->registerTEvent(20)<0)
  {
    std::cout<<"register error!"<<endl;
  }
  else
    std::cout<<"register ok!"<<endl;  
}

int ClientAgent::sendData()
{
  return 0;
}

int ClientAgent::recvData()
{
  return 0;
}

int ClientAgent::doAction(unsigned int tvalue)
{
  cout<<"doAction is executed here!"<<endl;
  return 0;
}

int ClientAgent::doAlways()
{
  cout<<"doAlways is executed here!"<<endl;
  return 0;
}

int ClientAgent::doDestroy()
{
  cout<<"doDestroy is executed here!"<<endl;
  return 0;
}

ClientAgent::~ClientAgent()
{
  delete m_pEpollEvent;
}
