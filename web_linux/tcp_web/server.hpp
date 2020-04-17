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



class server
{
  private:
    std::string ip;
    short port;
    int sockfd;
  public:
    server(std::string _ip,short _port):ip(_ip),port(_port),sockfd(-1)
    {}
    void InitServer()
    {
      sockfd = socket(AF_INET,SOCK_STREAM,0);
      if(sockfd < 0)
      {
        std::cout << "socket error" << std::endl;
        exit(1);
      }
      struct sockaddr_in local;
      bzero(&local,sizeof(local));

      local.sin_family = AF_INET;
      local.sin_addr.s_addr = inet_addr(ip.c_str());
      local.sin_port = htons(port);

      if(bind(sockfd,(struct sockaddr*)&local,sizeof(local)) < 0)
      {
        std::cout << "bind error" << std::endl;
        exit(2);
      }
      if(listen(sockfd,5) < 0)
      {
        std::cout << "listen error " << std::endl;
        exit(3);
      }

    }
    void ServiceIO(int fd)
    {
      while(1)
      {
        char buf[1024];
        ssize_t s = read(fd,buf,sizeof(buf));
        if(s > 0)
        {
          buf[s] = 0;
          std::cout << buf <<  std::endl;
        }
        else if(s == 0){
          std::cout << "client quit" << std::endl;
          exit(0);
        }
        else{
          std::cout << "read error" << std::endl;
          exit(4);
        }

        write(fd,buf,s);
      }
    }
    void Run()
    {
      for(;;)
      {
        struct sockaddr_in peer;
        socklen_t len = sizeof(peer);
        int fd = accept(sockfd,(struct sockaddr*)&peer,&len);
        if(fd < 0)
        {
          std::cout << "accept error" << std::endl;
          exit(3);
        }
        ServiceIO(fd);
      }
    }
    ~server()
    {
      if(sockfd > 0)
      {
        close(sockfd);
      }
    }
};
