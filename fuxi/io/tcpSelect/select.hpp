#pragma once

#include<iostream>
#include<string>
#include<strings.h>
#include<sys/select.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<vector>

#define MAX sizeof(fd_set)*8 

class selectServer{
private:
  int sockfd;
  std::string ip;
  short port;
  std::vector<int> vfds;

public:
  selectServer(std::string _ip = "127.0.0.1",short _port = 8080):sockfd(0),ip(_ip),port(_port),vfds(MAX,-1)
  {}
  ~selectServer()
  {}

  void selectInit()
  {
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in local;
    bzero(&local,sizeof(local));
    local.sin_addr.s_addr = inet_addr(ip.c_str());
    local.sin_port = htons(port);
    local.sin_family = AF_INET;
    if(sockfd < 0)
    {
      std::cout << "socket error" << std::endl;
      exit(2);
    }
    if(bind(sockfd,(struct sockaddr*)&local,sizeof(local)) < 0)
    {
      std::cout << "bind error" << std::endl; 
      exit(3);
    }

    if(listen(sockfd,5) < 0)
    {
      std::cout << "listen error" << std::endl;
      exit(4);
    }
    
    int opt = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  }
  void handlerEvent(fd_set *rfds)
  {
    for(int i = 0;i < sizeof(fd_set) * 8;i++)
    {
      if(vfds[i] < 0)
        continue;
      if(FD_ISSET(vfds[i],rfds))
      {
        if(vfds[i] == sockfd)
        {
          // 以一个新的连接到来了
          std::cout << "get a new link" << std::endl;
          struct sockaddr_in peer;
          bzero(&peer,sizeof(peer));
          socklen_t len = sizeof(peer);
          int sock = accept(sockfd,(sockaddr*)&peer,&len);
          if(sock < 0)
          {
            std::cout << "accept error" << std::endl;
            continue;
          }
          int j = 0;
          for(;j < MAX;j++)
          {
            if(vfds[j] < 0)
              break;
          }
          if(j == MAX - 1)
          {
            std::cout << "select full" << std::endl;
            exit(6);
          }
          else 
          {
            vfds[j] = sock;
          }

        }
        else 
        {
          //是一个io操作到来了
          std::cout << "get a new io " << std::endl;
          char buf[2014];

          ssize_t s = recv(vfds[i],buf,1023,0);
          if(s < 0)
          {
            std::cout << "recv error" << std::endl;
          }
          else if(s > 0)
          {
            buf[s] = 0;
            std::cout << "get information # " << buf << std::endl;
          }
          else 
          {
            std::cout << "peer close link" << std::endl;
            close(vfds[i]);
            vfds[i] = -1;
          }

        }
      }
      else 
      {
        std::cout << "don't have any select already" << std::endl;
      }
    }
    
  }
  void start()
  {
    vfds[0] = sockfd;
    fd_set rfds;

    for(;;)
    {
      int maxfd = 0;
      FD_ZERO(&rfds);
      for(auto it = vfds.begin();it != vfds.end();it++)
      {
        if(*it < 0)
          continue;
        FD_SET(*it,&rfds);
        if(*it > maxfd)
          maxfd = *it;
      }
      std::cout << "maxfd is > " << maxfd << std::endl;

      switch(select(maxfd + 2,&rfds,nullptr,nullptr,nullptr)) 
      {
        case -1:
          std::cout << "select error" << std::endl;
          break;
        case 0:
          std::cout << "time out" << std::endl;
          break;
        default:
          std::cout << "get a event" << std::endl;
          handlerEvent(&rfds);
          break;
      }
    }
  }


};

