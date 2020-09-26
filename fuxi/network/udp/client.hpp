#pragma once 

#include<iostream>
#include<string>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<strings.h>
#include<unistd.h>

class udpClient{
public:
  udpClient(std::string _ip = "127.0.0.1",short _port = 8080):port(_port),ip(_ip)
  {}

  ~udpClient()
  {
    if(sockfd > 0)
    {
      close(sockfd);
      sockfd = -1;
    }
  }
  void initClient()
  {
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
      std::cout << "create sockfd error" << std::endl;
      exit(2);
    }
  }
  void runClient()
  {
    // setting peer
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    peer.sin_addr.s_addr = inet_addr(ip.c_str());
    peer.sin_port = htons(port);
    peer.sin_family = AF_INET;
    char buf[1024];

    for(;;)
    {
      std::cout << "Please input information #" ;
      std::cin >> buf;
      sendto(sockfd,buf,strlen(buf),0,(struct sockaddr*)&peer,len);
    }
  }


private:
  int sockfd;
  short port;
  std::string ip;
};
