#include<iostream>
#include<unistd.h>
#include<sys/epoll.h>
#include<stdlib.h>

using namespace std;

int main()
{
  int epollfd = epoll_create(1);
  if(epollfd < 0)
  {
    cout << "epoll create error" << endl;
    exit(1);
  }
  struct epoll_event epEvent;
  epEvent.data.fd = 0;
  epEvent.events = EPOLLIN;
  int epctlret = epoll_ctl(epollfd,EPOLL_CTL_ADD,0,&epEvent);
  if(epctlret < 0)
  {
    cout << "epoll ctl error" << endl;
    exit(2);
  }
  epoll_event events[20];

  while(1)
  {
    int nfds = epoll_wait(epollfd,events,sizeof(events)/sizeof(events[0]),-1);
    if(nfds < 0)
    {
      cerr << "epoll wait error" << endl;
      exit(3);
    }
    for(int i = 0;i < nfds;i++)
    {
      char buf[50];
      read(events[nfds].data.fd,buf,sizeof(buf)/sizeof(buf[0]));
      cout << "get inf:" << buf << endl;
    }

  }


  return 0;
}
