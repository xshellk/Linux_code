#pragma once 

#include<sys/epoll.h>
#include<vector>
#include"Log.hpp"
#include"ThreadPool.hpp"
#include"Protocol.hpp"

#define EPOLL_SIZE 10

class Epoll{
public:
  bool Add(const int fd)
  {
    epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
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
      
      Task t;
      t.SetSock(fd,Entry::HanderRequest);
      tp->PushTask(t);
      //sleep(1);
      usleep(10000);
      //任务已经创建好了, 可以引入线程池了
      //线程池因为提前绑定好了,可以直接使用
      //内置任务到线程池中
      // 把fd给线程, 然后唤醒线程, 进行读取
      // 输出也利用epoll进行控制
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

};
