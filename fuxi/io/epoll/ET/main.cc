#include"epoll.hpp"

int main()
{
  ltEpoll *p = new ltEpoll;
  p->initServer();
  p->start();

  return 0;
}
