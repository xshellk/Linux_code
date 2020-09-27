#pragma once 

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include"threadPool.hpp"

class tcpServer{
public:
  tcpServer(std::string _ip,short _port):ip(_ip),port(_port),sockfd(-1)
  {}
  ~tcpServer()
  {
    if(sockfd > 0)
    {
      close(sockfd);
    }
  }

  void initServer()
  {
    multiThread.initThreadPool();
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
      std::cout << "create sockfd error" << std::endl;
      exit(2);
    }
    struct sockaddr_in local;
    bzero(&local,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = inet_addr(ip.c_str());
    local.sin_port = htons(port);

    if(0 != bind(sockfd,(struct sockaddr*)&local,sizeof(local)))
    {
      std::cout << "bind error" << std::endl; 
      exit(3);
    }
    if(0 != listen(sockfd,5))
    {
      std::cout << "listen error" << std::endl;
      exit(3);
    }
    std::cout << "server init success" << std::endl;
  }
  static void serverIo(void *arg)
  {
    char buf[1024];
    int *fd = (int *)arg;

    while(1)
    {
      ssize_t s = read(*fd,buf,sizeof(buf) - 1);
      if(s < 0)
      {
        std::cout << "write error" << std::endl;
        exit(5);
      }
      buf[s] = 0;
      std::cout << "Server get information is # " << buf << std::endl;
    }
  }

  void runServer()
  {
    std::cout << "server running" << std::endl;
    while(true)
    {
      struct sockaddr_in peer;
      bzero(&peer,sizeof(peer));
      socklen_t len = sizeof(peer);

      std::cout << "debug :ready accept" << std::endl;
      int peer_fd = accept(sockfd,(struct sockaddr*)&peer,&len);
      std::cout << "debug :done accept" << std::endl;

      if(peer_fd < 0)
      {
        std::cout << "accept error" << std::endl;
        exit(4);
      }
      else{
        std::cout << "get accept" << std::endl;
      }

      Task t(serverIo,&peer_fd);
      multiThread.pushTask(t);
    }

  }



private:
  short port;
  std::string ip;
  int sockfd;
  threadPool multiThread;

};
