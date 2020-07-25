#pragma once 

#include"Sock.hpp"
#include"Protocol.hpp"
#include"ThreadPool.hpp"
#include<iostream>
#include<pthread.h>
#include"Sock.hpp"
#include<unistd.h>


#define Default_port 8081

using namespace std;

class HttpServer{
  private:
    short port;
    int listen_sock;

    ThreadPool* tp;
  
  public:
    HttpServer(int _port = Default_port):port(_port),listen_sock(-1)
    {}
    ~HttpServer()
    {
      if(listen_sock > 0)
        close(listen_sock);
    }
  
  public:
    void InitServer()
    {
      listen_sock = Sock::CreatSock();
      Sock::Bind(listen_sock,port);
      Sock::Listen(listen_sock);
      signal(SIGPIPE,SIG_IGN);

      tp = new ThreadPool;
      tp->InitThreadPool();
    }
    void Start()
    {
      LOG(Normal,"server start success!");
      for(;;)
      {
        int sock = Sock::Accept(listen_sock);
        if(sock > 0)
        {
          LOG(Normal,"Get a new link");
          //pthread_t tid;
          //int *p = new int(sock);
          //pthread_create(&tid,nullptr,Entry::HanderRequest,(void *)p);
          //pthread_detach(tid);
          Task t;
          t.SetSock(sock,Entry::HanderRequest);
          tp->PushTask(t);
        }
        else 
        {

        }
      }
    }
};
