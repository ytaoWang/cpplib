#include "ClientAgent.h"

int main(int argc,char *argv[])
{
  Epoll epoll;
  ClientAgent client(&epoll);
  epoll.run();
  
  return 0;
}
