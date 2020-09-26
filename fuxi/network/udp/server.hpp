#pragma once 

#include<iostream>
#include<string>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<strings.h>

class udpServer{
public:
  udpServer(std::string _ip,short _port):port(_port),ip(_ip)
  {}

  ~udpServer()
  {}
  void initServer()
  {
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
      std::cout << "create sockfd error" << std::endl;
      exit(2);
    }
    struct sockaddr_in local;
    bzero(&local,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = inet_addr(ip.c_str());

    if(0 != bind(sockfd,(struct sockaddr*)&local,sizeof(local)))
    {
      std::cout << "bind error" << std::endl;
      exit(3);
    }
    std::cout << "bind success" << std::endl;
  }
  void runServer()
  {
    // setting peer
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    bzero(&peer,sizeof(peer));
    char buf[1024];

    for(;;)
    {
      std::cout << "doing recvfrom " << std::endl;
      ssize_t size = recvfrom(sockfd,buf,sizeof(buf) - 1,0,(struct sockaddr*)&peer,&len);
      if(size > 0)
      {
        buf[size] = 0;
        std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "server get informatiom from client #" << buf << std::endl << std::endl;
        std::cout << "client ip:port is #" << inet_ntoa(peer.sin_addr) << ":" << peer.sin_port << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;
      }
      else{
        std::cout << "recvfrom error" << std::endl;
        exit(4);
      }
    }
  }


private:
  int sockfd;
  short port;
  std::string ip;
};
