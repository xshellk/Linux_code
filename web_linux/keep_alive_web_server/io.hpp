#pragma once 

#include<sys/epoll.h>
#include<vector>
#include<unordered_set>
#include"Log.hpp"
#include"ThreadPool.hpp"
#include"Protocol.hpp"

#define EPOLL_SIZE 1000

class Epoll{
private:
  bool Check(int fd)
  {
    unordered_set<int>::iterator it = RunThreadFd.find(fd);
    if(it == RunThreadFd.end())
    {
      return false;
    }
    RunThreadFd.insert(fd);
    return true;
  }

  bool SetNoBlock(int fd)
  {
    int fl = fcntl(fd,F_GETFL);
    if(fl < 0)
    {
      LOG(Warning,"get fl to set no block error");
      return false;
    }
    int ret = fcntl(fd,F_SETFL | O_NONBLOCK);
    if(ret < 0)
    {
      LOG(Warning,"set no block error");
      return false;
    }
    return true;
  }
public:
  bool Add(const int fd)
  {
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    SetNoBlock(fd);
    int ret = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&ev);
    if(ret < 0)
    {
      LOG(Fatal,"epoll add error");
      return false;
    }
    return true;
  }
  bool Del(const int fd)
  {
    int ret = epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,NULL);
    if(ret < 0)
    {
      LOG(Fatal,"epoll del error");
      return false;
    }
    return true;
  }
  
  bool Wait(ThreadPool *tp)
  {
    epoll_event epoll_events[EPOLL_SIZE];
    int nfds = epoll_wait(epoll_fd,epoll_events,EPOLL_SIZE,-1);
    
    if(nfds < 0)
    {
      LOG(Fatal,"epoll wait error");
      return false;
    }
    cout << "debug nfds is >" <<  nfds << endl;;
    for(int i = 0;i < nfds;i++)
    {
      int fd = epoll_events[i].data.fd;
      cout << "dubug,get success fd is :" << fd << endl;
      
      if(Check(fd))//检测是否该fd已经被线程去进行读取了
      {
        continue;
      }
      Task t;
      t.SetSock(fd,Entry::HanderRequest);
      t.SetDoingFd(RunThreadFd);

      tp->PushTask(t);
      //sleep(1);
      usleep(10000);
      //cout << "debug, RunThreadFd size > " << RunThreadFd.bucket_count() << endl;
    }
    return true;
  }

  Epoll()
  {
    epoll_fd = epoll_create(EPOLL_SIZE);
  }
  ~Epoll()
  {}

private:
  int epoll_fd;
  unordered_set<int> RunThreadFd;

};
