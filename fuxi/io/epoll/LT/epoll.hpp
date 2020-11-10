#pragma once 

#include<iostream>
#include<sys/epoll.h>
#include<unistd.h>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<strings.h>


class epollServer{
private:
  int listen_sock;
  int epfd;
  std::string ip;
  short port;

public:
  epollServer():listen_sock(-1),epfd(-1),ip("127.0.0.1"),port(8080)
  {}
  ~epollServer() 
  {
    if(epfd > 0)
    {
      close(epfd);
      epfd = -1;
    }
  }
  void initServer()
  {
    struct sockaddr_in local;
    bzero(&local,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = inet_addr(ip.c_str());
    listen_sock = socket(AF_INET,SOCK_STREAM,0);

    if(listen_sock < 0)
    {
      std::cout << "socket error" << std::endl;
      exit(2);
    }
    if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local)) < 0)
    {
      std::cout << "bind error" << std::endl;
      exit(3);
    }
    if(listen(listen_sock,5) < 0)
    {
      std::cout << "listen error" << std::endl;
      exit(4);
    }
    int opt = 1;
    setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  }
  void start()
  {
    epoll_event epv;
    epfd = epoll_create(10);
    epv.events = EPOLLIN;
    epv.data.fd = listen_sock;
    if(epfd < 0)
    {
      std::cout << "epfd create error" << std::endl;
      exit(5);
    }
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&epv);
    if(ret < 0)
    {
      std::cout << "epoll ctl error in start" << std::endl;
      exit(6);
    }
    epoll_event retEpv[10];
    std::cout << "begin do epoll wait " << std::endl;

    for(;;)
    {
      int nfds = epoll_wait(epfd,retEpv,sizeof(retEpv)/sizeof(retEpv[0]),-1);

      std::cout << "epoll wait ret is " << nfds << std::endl;

      for(int i = 0;i < nfds;i++)
      {
        handlerEvent(retEpv->data.fd);
      }

    }
    

    
  }
  void handlerEvent(int fd)
  {
    if(fd == listen_sock)
    {
      //get a new link
      
      struct sockaddr_in peer;
      socklen_t len = sizeof(peer);
      bzero(&peer,len);
      int sock = accept(listen_sock,(sockaddr*)&peer,&len);
      if(sock < 0)
      {
        std::cout << "accept error" << std::endl;
        exit(7);
      }

      std::cout << "accept success" << std::endl;
      epoll_event ev;
      ev.data.fd = sock;
      ev.events = EPOLLIN;

      int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,sock,&ev);

      if(ret < 0)
      {
        std::cout << "epoll ctl error" << std::endl;
        //std::cout << "sock fd is " << sock << std::endl;
        exit(8);
      }
      std::cout << "epoll success" << std::endl;
    }
    else 
    {
      char buf[1024];
      ssize_t s = recv(fd,buf,1023,0);
      if(s < 0)
      {
        std::cout << "recv error" << std::endl;
        exit(9);
      }else if(s == 0)
      {
        std::cout << "peer close link" << std::endl;
        epoll_ctl(epfd,EPOLL_CTL_DEL,fd,nullptr);
        return;
      }
      buf[s] = 0;
      std::cout << "buf is " << buf << std::endl;

    }
  }

};
