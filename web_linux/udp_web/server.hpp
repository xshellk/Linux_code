#pragma once 

#include<iostream>
#include<unistd.h>
#include<stdlib.h>
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
    int sockfd;
  public:
    server(std::string _ip = "127.0.0.1",short _port = 8080) : port(_port),ip(_ip)
    {

    }
    void InitServer()
    {
      sockfd = socket(AF_INET,SOCK_DGRAM,0);
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
    void run()
    {
      char buf[1024] = {0};
      struct sockaddr_in peer;
      for(;;)
      {
        socklen_t len = sizeof(peer);
        ssize_t size = recvfrom(sockfd,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&len);
        if(size > 0)
        {
          buf[size] = 0;
          std::string client_ip = inet_ntoa(peer.sin_addr);
          int client_port = ntohs(peer.sin_port);
          std::cout << "client ip #" << client_ip << "  client port # " << client_port << std::endl;
          std::cout << "client msg # :" << buf << std::endl; 
        }
        else{
          std::cout << "recvfrom error" << std::endl;
          exit(4);
        }
      }
    }
    ~server()
    {
      if(sockfd >= 0)
        close(sockfd);
    }
};


