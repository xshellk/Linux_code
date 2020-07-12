#pragma once 

#include"Sock.hpp"
#include"Protocol.hpp"
#include<iostream>
#include<pthread.h>

#define Default_port 8081

using namespace std;

class HttpServer{
  private:
    short port;
    int listen_sock;
  
  public:
    HttpServer(int _port = Default_port):port(_port),listen_sock(-1)
    {}
    ~HttpServer()
    {}
  
  public:
    void InitServer()
    {
      listen_sock = Sock::CreatSock();
      Sock::Bind(listen_sock,port);
      Sock::Listen(listen_sock);
    }
    void Start()
    {
      for(;;)
      {
        int sock = Sock::Accept(listen_sock);
        if(sock > 0)
        {
          LOG(Normal,"Get a new link");
          pthread_t tid;
          pthread_create(&tid,nullptr,Entry::HanderRequest,(void *)sock);
          pthread_detach(tid);

        }
        else 
        {

        }
      }
    }
};
