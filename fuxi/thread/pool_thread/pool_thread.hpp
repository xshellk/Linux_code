#pragma once 

#include<iostream>
#include<unistd.h>
#include<pthread.h>
#include<queue>
class Task
{
public:
  Task()
  {}
  ~Task()
  {}

public:
  void Run()
  {
    std::cout << "task doing something " << std::endl;
  }
};

class threadPool
{
private:
  bool taskIsEmpty()
  {
    return q.size() == 0;
  }
  void lockThread()
  {
    pthread_mutex_lock(&l);
  }
  void unlockThread()
  {
    pthread_mutex_unlock(&l);
  }
  void waitThread()
  {
    pthread_cond_wait(&c,&l);
  }
  void signalThread()
  {
    pthread_cond_signal(&c);
  }
  void getTask(Task &t)
  {
    t = q.front();
    q.pop();
  }
  
public:
  void pushTask(Task  &t)
  {
    q.push(t);
    signalThread();
  }
  static void * Routine(void *arg)
  {
    threadPool *self = (threadPool *)arg;
    while(true)
    {
      self->lockThread(); 

      while(self->taskIsEmpty())
      {
        self->waitThread();
      }
      self->signalThread();
      Task t;
      self->getTask(t);

      self->unlockThread();
      t.Run();
    }

  }
  void pthreadInit()
  {
    pthread_t tid;
    for(int i = 0;i < cap;i++)
    {
      pthread_create(&tid,NULL,Routine,this);
    }
    pthread_cond_init(&c,NULL);
    pthread_mutex_init(&l,NULL);
  }
  threadPool(int _cap = 4): cap(_cap)
  {}
  ~threadPool()
  {
      pthread_mutex_destroy(&l);
      pthread_cond_destroy(&c);
  }

private:
  int cap;
  pthread_cond_t c;
  pthread_mutex_t l;
  std::queue<Task> q;

};
