#pragma once 

#include"Log.hpp"

#include<sys/socket.h>
#include<sys/types.h>
#include<strings.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string>

class Sock
{
public:
  Sock()
  {}
  ~Sock()
  {}
  static int CreatSock()
  {
    int listen_sock = socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock < 0)
    {
      LOG(Fatal,"creat sock error");
      exit(SOCK_CREAT);
    }
    return listen_sock;
  }
  static void Bind(int sock,short port)
  {
    struct sockaddr_in local;
    bzero(&local,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
    {
      LOG(Fatal,"bind error");
      exit(BIND_ERROR);
    }
  }
  static void Listen(int sock)
  {
    if(listen(sock,BACK_LOG) < 0)
    {
      LOG(Fatal,"listen error");
      exit(LISTEN_ERROR);
    }
  }

  static int Accept(int sock)
  {
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    int ret_sock = accept(sock,(struct sockaddr*)&peer,&len);
    if(ret_sock < 0)
    {
      LOG(Error,"accept error");
    }
    return ret_sock;
  }


private:
};
