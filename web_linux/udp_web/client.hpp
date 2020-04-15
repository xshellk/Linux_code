#pragma once 

#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<strings.h>
#include<string>

class client{
  private:
    short peer_port;
    std::string peer_ip;
    int sockfd;
  public:
    client(std::string _ip = "127.0.0.1",short _port = 8080):peer_ip(_ip) , peer_port(_port)
    {}
    void InitClient()
    {
      sockfd = socket(AF_INET,SOCK_DGRAM,0);
      if(sockfd < 0)
      {
        std::cout << "socket error" << std::endl;
        exit(2);
      }
    }
    void run()
    {
      std::string msg;
      struct sockaddr_in server;
      server.sin_addr.s_addr = inet_addr(peer_ip.c_str());
      server.sin_port = htons(peer_port);
      while(1)
      {
      socklen_t len = sizeof(server);

      std::cout << "请输入信息 #" ;
      std::cin >> msg;

      ssize_t s = sendto(sockfd,msg.c_str(),msg.size(),0,(struct sockaddr*)&server,len);
      }
    }

    ~client()
    {
      if(sockfd > 0)
      {
        close(sockfd);
      }
    }
};
