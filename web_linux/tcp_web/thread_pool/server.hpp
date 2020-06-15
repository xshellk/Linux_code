#pragma once 

#include"protocol.hpp"
#include<iostream>

class Server 
{
  private:
    std::string ip;
    short port;
    int sockfd;
  public:
    Server(std::string _ip,short _port) : ip(_ip),port(_port) ,sockfd(-1)
    {}
    void InitServer()
    {
      sockfd = Sock::sock();
      Sock::Bind(sockfd,ip,port);
      Sock::Listen(sockfd);
    } 

    void Start()
    {
      for(;;)
      {
        int sock = Sock::Accept(sockfd);
        if(sock < 0)
        {
          std::cerr << "Accept error" << std::endl;
        }
        std::string msg;
        Sock::Recv(sockfd,msg);
        Sock::Send(sockfd,msg) ;
        std::cout << msg << std::endl;
        // threadpool

      }
    }

    ~Server()
    {
      if(sockfd > 0)
      {
        close(sockfd);
        sockfd = -1;
      }
    }
};


