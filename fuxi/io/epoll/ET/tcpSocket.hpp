#pragma once 

#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>
#include<string>
#include<arpa/inet.h>

class tcpSocket{
private:
  int sockfd;
  std::string ip;
  short port;
public:
  tcpSocket():sockfd(-1),ip("127.0.0.1"),port(8080)
  {}
  ~tcpSocket()
  {
    if(sockfd > 0)
      close(sockfd);
  }
   
public:
  
  void createSocket()
  {
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
      std::cout << "create socket error" << std::endl;
      exit(2);
    }
    std::cout << "create socket success"<< std::endl;
  }
  void bindSocket()
  {
    struct sockaddr_in local;
    bzero(&local,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = inet_addr(ip.c_str());
    if(bind(sockfd,(struct sockaddr*)&local,sizeof(local)) < 0)
    {
      std::cout << "bind error" << std::endl;
      exit(3);
    }
    std::cout << "bind success" << std::endl;
  }
  void listenSocket(int count)
  {
    if(listen(sockfd,count) < 0)
    {
      std::cout << "listen error" << std::endl;
    }
    std::cout << "listen success " << std::endl;
  }
  void acceptSocket(int *fd,struct sockaddr_in *peer)
  {
    socklen_t len = sizeof(*peer);
    *fd = accept(sockfd,(struct sockaddr*)peer,&len);
    if(*fd < 0)
    {
      std::cout << "accept error" << std::endl;
      exit(4);
    }
    std::cout << "accept success" << std::endl;
  }
  
 




public:
  int getFd()
  {
    return sockfd;
  }
  void setIp(std::string str)
  {
    ip = str;
  }
  void setPort(short num)
  {
    port = num;
  }

  
  

};
