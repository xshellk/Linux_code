#pragma once 


#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<queue>

#include"io.hpp"
#include"Log.hpp"
#include"Sock.hpp"
#include"Protocol.hpp"
#include"ThreadPool.hpp"
#include"Sock.hpp"

#define Default_port 8081

using namespace std;
class KeepAliveLink
{
public:


private:

};

class HttpServer{
  private:
    short port;
    int listen_sock;

    ThreadPool* tp;
    queue<int> KPspace;
    myEpoll* ep;
  
  public:
    HttpServer(int _port = Default_port):port(_port),listen_sock(-1)
    {}
    ~HttpServer()
    {
      if(listen_sock > 0)
        close(listen_sock);
    }
    static void *CheckLinkThread(void *args)
    {
      HttpServer* server = (HttpServer *)args;
      for(;;)
      {

      }
    }
    static void *EpollThread(void * args)
    {
      HttpServer* server = (HttpServer *)args;
      for(;;)
      {
        //bool ret = server->ep->Wait(server->tp,server->ep);
        bool ret = server->ep->Wait(server->tp);
        if(!ret)
        {
          LOG(Error,"epoll wait error");
        }

      }
    }

    void PushKeepSpace(int _sock)
    {
      KPspace.push(_sock);
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
      ep = new myEpoll;
      
      pthread_t pid;
      pthread_create(&pid,nullptr,EpollThread,this);

      //通过另外的线程进行链接的检测
      //pthread_create(&pid,nullptr,CheckLinkThread,this);

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
          
          //old do
          //Task t;
          //t.SetSock(sock,Entry::HanderRequest);
          //tp->PushTask(t);

          //new do
          //将链接的fd放入数组空间保存
          PushKeepSpace(sock);
          cout << "debug new sock is :" << sock << endl;
          //进行epoll模型的注册
          ep->Add(sock);
        }
        else 
        {

        }
      }
    }
};
