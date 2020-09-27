#pragma once 

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdlib.h>
#include<strings.h>
#include<string.h>

class tcpClient{
public:
  tcpClient(std::string _ip = "127.0.0.1",short _port = 8080):ip(_ip),port(_port),sockfd(-1)
  {}
  ~tcpClient()
  {
    if(sockfd > 0)
    {
      close(sockfd);
    }
  }

  void initClient()
  {
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd < 0)
    {
      std::cout << "create sockfd error" << std::endl;
      exit(2);
    }
    
  }
  void runClient()
  {

    while(1)
    {
      char buf[1024];
      std::cout << "Please input:# " ;
      std::cin >> buf;
      write(sockfd,buf,strlen(buf));
    }
  }

  void connect()
  {
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if(::connect(sockfd,(struct sockaddr*)&server,sizeof(server)) == 0)
    {
      std::cout << "connect success" << std::endl;
    }
    else{
      std::cout << "connect error" << std::endl;
      exit(6);
    }

  }



private:
  short port;
  std::string ip;
  int sockfd;

};
