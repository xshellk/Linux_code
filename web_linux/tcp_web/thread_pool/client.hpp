#pragma once 

#include"protocol.hpp"
#include<iostream>

class Client 
{
  private:
    std::string ip;
    short port;
    int sockfd;
  public:
    Client(std::string _ip,short _port) : ip(_ip),port(_port) ,sockfd(-1)
    {}
    void InitClient()
    {
      sockfd = Sock::sock();
      Sock::Connect(sockfd,ip,port);
    } 

    void Start()
    {
      for(;;)
      {
        std::string output;
        std::string msg;
        std::cout << "请输入#";
        std::cin>> msg;
        Sock::Send(sockfd,msg);
        if(Sock::Recv(sockfd,output))
        {
          std::cout << "服务器会写# " << output << std::endl;
        }
        else{
          break;
        }

      }
    }

    ~Client()
    {
      if(sockfd > 0)
      {
        close(sockfd);
        sockfd = -1;
      }
    }
};


