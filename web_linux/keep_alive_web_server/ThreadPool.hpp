#pragma once 

#include<iostream>
#include<queue>
#include<pthread.h>
#include<unistd.h>
#include<unordered_set>
//#include"common.hpp"
#include"io.hpp"


using namespace std;

typedef void *(*Handler_t)(void *args);

class Task{
  private:
    int sock;
    Handler_t h;
    //myEpoll *ep;
    unordered_set<int> *doingFd;
  public:
    
    Task(int _sock = -1):sock(_sock)
    {}
    void SetSock(int _sock,Handler_t handler)
    {
      sock = _sock;
      h = handler;
    }
    int GetFd()
    {
      return sock;
    }
    void Run()
    {
      // 构造一个内部类, 将sock和*ep放进去, 然后根绝退出码来选择是否从epoll模型中清除
      
      //ThreadParameter tmp;
      //tmp.threadParameterEp = ep;
      //tmp.threadParameterSock = sock;
      //h((void*)&tmp);
      h((void*)&sock);

    }
    void FreeDoingFd()
    {
      auto delP = doingFd->find(sock);
      if(delP != doingFd->end())
      {
        doingFd->erase(delP);
      }
    }
    void SetDoingFd(unordered_set<int> *p)
    {
      doingFd = p;
    }
    //void SetEpoll(myEpoll *_ep)
    //{
    //  ep = _ep;
    //}
    
    ~Task()
    {}
};

class ThreadPool{
  private:
    int num;
    queue<Task> q;
    pthread_mutex_t lock;
    pthread_cond_t cond;

  public:
    ThreadPool(int _num = 1):num(_num)
    {
      pthread_mutex_init(&lock,nullptr);
      pthread_cond_init(&cond,nullptr);
    }
    void LockQueue()
    {
      pthread_mutex_lock(&lock);
    }
    void UnlockQueue()
    {
      pthread_mutex_unlock(&lock);
    }
    void ThreadWait()
    {
      pthread_cond_wait(&cond,&lock);
    }
    bool IsQueueEmpty()
    {
      return q.empty();
    }
    void WakeupThread()
    {
      pthread_cond_signal(&cond);
    }
    void PushTask(Task t)
    {
      LockQueue();
      q.push(t);
      UnlockQueue();
      WakeupThread();
    }
    Task PopTask()
    {
      Task ret = q.front();
      q.pop();
      return ret;
    }
    static void *Routine(void *args)
    {
      ThreadPool*tp = (ThreadPool*)args;

      while(true)
      {
        tp->LockQueue();
        while(tp->IsQueueEmpty())
        {
          tp->ThreadWait();
        }
        Task t = tp->PopTask();
        tp->UnlockQueue();
        t.Run();
        t.FreeDoingFd();

      }
    }
    void InitThreadPool()
    {
      pthread_t pid;
      num = (int)sysconf(_SC_NPROCESSORS_ONLN);
      for(auto i = 0;i < num;i++)
      {
        pthread_create(&pid,nullptr,Routine,this);
        pthread_detach(pid);
      }
    }
    ~ThreadPool()
    {
      pthread_mutex_destroy(&lock);
      pthread_cond_destroy(&cond);
    }


};
