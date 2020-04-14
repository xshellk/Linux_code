#pragma once 

#include<iostream>
#include<unistd.h>

#include<string>

#include<pthread.h>
#include<queue>

#define NUM 5

class Task{

private:
  int x;
  int y;
  char op;
public:
  Task()
  {
    x = 1;
    y = 2;
    op = '+';
  }
  Task(int _x,int _y,char _op):x(_x),y(_y),op(_op)
  {}
  ~Task()
  {}
  void run()
  {
    int ret; 
    switch(op)
    {
      case '+':
        ret = x + y;
        break;
      case '-':
        ret = x - y;
        break;
      case '*':
        ret = x * y;
      case '/':
        if(y == 0)
        {
          ret = 0;
        }
        else{
          ret = x/y;
        }
        break;
      default:
        break;
    }
    std::cout << " count ret :" << ret << std::endl;
  }
};

class threadPool{
private:
  int num;
  std::queue<Task> q;

  pthread_mutex_t lock;
  pthread_cond_t cond;
private:
  void LockTaskQueue()
  {
    pthread_mutex_lock(&lock);
  }
  void UnlockTaskQueue()
  {
    pthread_mutex_unlock(&lock);
  }
  void ThreadWait()
  {
    pthread_cond_wait(&cond,&lock);
  }
  void Thread_Signal()
  {
    pthread_cond_signal(&cond);
  }
  void PopTask(Task &data) 
  {
    data = q.front();
    q.pop();
  }
public:
  void PushTask(Task &t)
  {
    q.push(t);
    Thread_Signal();

  }
private:
  bool TaskIsEmpty()
  {
    return q.size() == 0;
  }
  static void *Routine(void * arg)
  {
    threadPool *self = (threadPool *)arg;
    while(true)
    {
      self->LockTaskQueue();
      while(self->TaskIsEmpty())
      {
        self->ThreadWait();
      }

      Task t;
      self->PopTask(t);
      self->UnlockTaskQueue();
      t.run();

    }
  }
public:
  threadPool(int _num = NUM):num(_num)
  {}
  ~threadPool()
  {
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
  }
  void InitPthreadPool()
  {
    pthread_t tid;
    for(int i = 0;i < num;i++)
    {
      pthread_create(&tid,nullptr,Routine,this);
    }
    pthread_mutex_init(&lock,nullptr);
    pthread_cond_init(&cond,nullptr);
  }
};

