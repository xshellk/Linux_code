#pragma once 

#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<strings.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define LISTEN 5
class Sock
{
  public:            
  static int sock()
  {
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
      std::cerr << "socket error" << std::endl;
      exit(2);
    }
    //std::cout << sockfd << std::endl;
    return sockfd;
  }
  static void Bind(int &sockfd,std::string ip,short port)
  {
    struct sockaddr_in local;

    bzero(&local,sizeof(local));

    local.sin_addr.s_addr = inet_addr(ip.c_str());
    local.sin_port = htons(port);
    local.sin_family = AF_INET;
  
    socklen_t len = sizeof(local);
    std::cout << ip << ":" << port << std::endl;
    
    //std::cout << bind(sockfd,(struct sockaddr*)&local,len) << std::endl;
    if(bind(sockfd,(struct sockaddr*)&local,len) == 0)
    {
      std::cout << "Bind exccess" << std::endl;
    }
    else 
    {
      std::cerr << "Bind error" << std::endl;
      exit(3);
    }
  }
  static void Listen(int &sockfd)
  {
    if(listen(sockfd,LISTEN) != 0)
    {
      std::cerr << "listen error" << std::endl;
      exit(4);
    }
  }
  static int Accept(int &sockfd)
  {
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    int _sock = accept(sockfd,(struct sockaddr*)&peer,&len);
    if(_sock < 0)
    {
      std::cerr << "accept error" << std::endl;
      exit(5);
    }
    else 
    {
      std::cout << "get a new accept" << std::endl;
    }
    return _sock;
  }
  static void Connect(int &sockfd,std::string ip,short port) 
  {
    struct sockaddr_in peer;
    bzero(&peer,sizeof(peer));

    peer.sin_addr.s_addr = inet_addr(ip.c_str());
    peer.sin_port = htons(port);
    peer.sin_family = AF_INET;

    if(connect(sockfd,(struct sockaddr*)&peer,sizeof(peer)) < 0)
    {
      std::cerr << "connect error" << std::endl;
      exit(5);
    }
  }
  
  static bool Recv(int &sockfd,std::string &output)
  {
    char buf[1024];
    bool ret = true;
    ssize_t s = read(sockfd,buf,sizeof(buf) - 1);
    if(s > 0)
    {
      buf[s] = 0;
      output = buf;
    }
    else if(s == 0)
    {
      std::cout << "other close connect" << std::endl;
      close(sockfd);
      ret = false;
    }
    else 
    {
      std::cerr << "read error" << std::endl;
      close(sockfd);
      ret = false;
    }
    return ret;
  }
  static void Send(int &sockfd,std::string &input)
  {
    write(sockfd,input.c_str(),input.size());
  }
};

