#pragma once 

#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<strings.h>
#include<stdlib.h>

class client
{
  private:
    std::string ip;
    short port;
    int sockfd;
  public:
    client(std::string _ip,short _port):ip(_ip),port(_port),sockfd(-1)
    {}
    void InitClient()
    {
      sockfd = socket(AF_INET,SOCK_STREAM,0);
      if(sockfd < 0)
      {
        std::cout << "InitClien  error" << std::endl;
        exit(2);
      }
      
      std::cout << "InitClient success" << std::endl;
    }
    void Connect()
    {
      struct sockaddr_in server;
      bzero(&server,sizeof(server));

      server.sin_addr.s_addr = inet_addr(ip.c_str());
      server.sin_family = AF_INET;
      server.sin_port = htons(port);
      socklen_t len = sizeof(server);
      if(connect(sockfd,(struct sockaddr*)&server,len) == 0)
      {
        std::cout << "connect success" << std::endl;
      }
      else{
        std::cout << "connect error" << std::endl;
        exit(3);
      }
    }
    void run()
    {
      char buf[1024];
      std::string msg;


      while(true)
      {
        std::cout << "请输入数据 #";
        std::cin >> msg;
        write(sockfd,msg.c_str(),msg.size());

        ssize_t s = read(sockfd,buf,sizeof(buf) - 1);
        if(s > 0)
        {
          buf[s] = 0;
          std::cout << "server ret #" << buf << std::endl;
        }
        else{
          std::cout  << "read error" << std::endl;
        }
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
