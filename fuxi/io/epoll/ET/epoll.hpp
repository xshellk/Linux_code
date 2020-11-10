#pragma once 

#include"tcpSocket.hpp"
#include<sys/epoll.h>

class ltEpoll{
private:
  int epfd;
  tcpSocket tcp;

public:
  void initServer()
  {
    //tcp server init begin
    tcp.createSocket();
    tcp.bindSocket();
    tcp.listenSocket(5);
    //tcp server init ok
   ////////////////////////////////////////////////////////////////////////// 
    //epfd io begin
    epfd = epoll_create(10);
    if(epfd < 0)
    {
      std::cout << "epfd create error" << std::endl;
      exit(1);
    }
    std::cout << "epfd create success" << std::endl;
    epoll_event ev;
    ev.data.fd = tcp.getFd();
    ev.events = EPOLLIN;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,tcp.getFd(),&ev) < 0)
    {
      std::cout << "epoll ctl error" << std::endl;
      exit(1);
    }
    std::cout << "epoll ctl success" << std::endl;
    //epoll init success
    ////////////////////////////////////////////////////////////////////////////////
  }
  void start()
  {
    epoll_event ev[10];
    for(;;)
    {
      int ret = epoll_wait(epfd,ev,sizeof(ev)/sizeof(ev[0]),-1);
      if(ret < 0)
      {
        std::cout << "epoll wait error" << std::endl;
        return ;
      }

      std::cout << "epoll wait success " << std::endl;
      for(int i = 0;i < ret;i++)
      {
        if(ev[i].data.fd == tcp.getFd())
        {
          std::cout << "get a new link" << std::endl;
          struct sockaddr_in peer;
          int fd;
          tcp.acceptSocket(&fd,&peer);
          epoll_event tmpev;
          tmpev.data.fd = fd;
          tmpev.events = EPOLLIN;
          if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&tmpev) < 0)
          {
            std::cout << "epoll ctl after accept errro" << std::endl;
            return;
          }
        }
        else 
        {
          std::cout << "--------------------------------------------" << std::endl;
          std::cout << "                 do once io                 " << std::endl;

          char buf[1024];
          ssize_t s = recv(ev[i].data.fd,buf,1023,0);
          if(s < 0)
          {
            std::cout << "recv error" <<std::endl;
          }else if(s == 0)
          {
            std::cout << "peer close link" << std::endl;
            epoll_ctl(epfd,EPOLL_CTL_DEL,ev[i].data.fd,nullptr);
          }

          buf[s] = 0;
          std::cout << "get information is #" << buf << std::endl;
          std::cout << "--------------------------------------------" << std::endl;
        }

      }

    }
  }
};
