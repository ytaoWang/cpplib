/**
 * @file   ClientAgent.h
 * @author wyt <wyt@wyt-laptop>
 * @date   Thu Feb  9 22:41:16 2012
 * 
 * @brief  ClientAgent for test Epoll+EpollEvent's function
 * @email:datao0907@163.com
 * 
 */

#ifndef __CLIENT_AGENT_H__
#define __CLIENT_AGENT_H__

#include "Agent.h"

class EpollEvent;
class Epoll;

class ClientAgent:public Agent
{
public:
  ClientAgent(Epoll *pEpoll);
  ~ClientAgent();
  int sendData();
  int recvData();
  int doAction(unsigned int);
  int doAlways();
  int doDestroy();
private:
  EpollEvent* m_pEpollEvent;
  
};



#endif
