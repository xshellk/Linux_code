#include"epoll.hpp"

using namespace std;

int main()
{

  epollServer *p = new epollServer();

  p->initServer();
  p->start();


  delete(p);
  return 0;
}
