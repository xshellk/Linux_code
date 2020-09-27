#pragma once 

#include<iostream>
#include<pthread.h>
#include<queue>
#include<unistd.h>

class Task{
public:
  void run()
  {
    std::cout << "taks doing something" << std::endl;
    fun(arg);
  }
  Task(void (*f)(void *) = NULL,void *_arg = NULL):fun(f),arg(_arg)
  {}
  ~Task()
  {}

private:
  void (*fun)(void *);
  void *arg;

};

class threadPool{
public:
  threadPool(int _cap = 3):cap(_cap)
  {
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond,NULL);
  }
  ~threadPool()
  {
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
  }
private: 
  bool isEmpty()
  {
    return q.size() == 0;
  }
  void lockThread()
  {
    pthread_mutex_lock(&lock);
  }
  void unlockThread()
  {
    pthread_mutex_unlock(&lock);
  }
  void waitThread()
  {
    pthread_cond_wait(&cond,&lock);
  }
  void signalThread()
  {
    pthread_cond_signal(&cond);
  }
  void getTask(Task &t)
  {
    t = q.front(); 
  }
  static void *Routine(void *arg)
  {
    threadPool* self = (threadPool *)arg;

    while(true)
    {

      self->lockThread();
      while(self->isEmpty())
      {
        self->waitThread();
      }

      Task t;
      self->getTask(t);
      t.run();
      self->unlockThread();
    }
  }

public:

  void pushTask(Task t)
  {
    q.push(t);
    signalThread();
  }
  void initThreadPool()
  {
    pthread_t tid;
    for(int i = 0;i < cap;i++)
    {
      pthread_create(&tid,NULL,Routine,this);
    }
    std::cout << "threadPool init success" << std::endl;
  }

private:
  std::queue<Task> q;
  pthread_mutex_t lock;
  pthread_cond_t cond;
  int cap;

};
