#pragma once 

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<strings.h>

#include<string>

class server
{
  private:
    short port;
    std::string ip;
  public:
    server(short _port = 8080,std::string _ip = "127.0.0.1") : port(_port),ip(_ip)
    {

    }
    void InitServer()
    {
      int sockfd = socket(AF_INET,SOCK_DGRAM,0);
      if(sockfd < 0)
      {
        std::cerr<< "socket error" << std::endl;
        exit(2);
      }
      struct sockaddr_in local;
      bzero(&local,sizeof(local));

      local.sin_family = AF_INET;
      local.sin_port = htons(port);
      local.sin_addr.s_addr = inet_addr(ip.c_str());

      if(bind(sockfd,(struct sockaddr*)&local,sizeof(local)) == 0)
      {
        std::cout << "run fun" << std::endl;
      }
      else{
        std::cerr << "bind error" << std::endl;
        exit(3);
      }

    }
    ~server()
    {

    }
};


