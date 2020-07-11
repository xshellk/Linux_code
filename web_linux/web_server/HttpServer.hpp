#pragma once 

#include"Sock.hpp"
#include<iostream>

using namespace std;

class HttpServer
{
  public:
    void Start(short port)
    {
      int sockfd = Sock::CreatSock();
      Sock::Bind(sockfd,port);
      Sock::Listen(sockfd);
      for(;;)
      {
        int sock = Sock::Accept(sockfd);
      }
    }
};
