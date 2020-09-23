#pragma once 

#include<iostream>
#include<pthread.h>
#include<queue>

class blockQueue{

public:
  void lockQueue()
  {
    pthread_mutex_lock(&lock);
  }
  void unlockQueue()
  {
    pthread_mutex_unlock(&lock);
  }

  bool isFull()
  {
    return q.size() == (size_t)cap;
  }
  bool isEmpty()
  {
    return q.size() == 0;
  }

  void pop(int *ret)
  {
    lockQueue();
    while(isEmpty())
    {
      pthread_cond_wait(&cond_c,&lock);
    }
    *ret = q.front();
    q.pop();

    unlockQueue();
    pthread_cond_signal(&cond_p);
  }
  void push(int num)
  {
    lockQueue();
    while(isFull())
    {
      pthread_cond_wait(&cond_p,&lock);
    }

    q.push(num);
    unlockQueue();
    pthread_cond_signal(&cond_c);
  }

  blockQueue(int _cap = 20):cap(_cap){
    pthread_mutex_init(&lock,NULL);
    pthread_cond_init(&cond_c,NULL);
    pthread_cond_init(&cond_p,NULL);
  }
  ~blockQueue(){
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_c);
    pthread_cond_destroy(&cond_p);
  }
private:
  pthread_mutex_t lock;
  pthread_cond_t cond_c;
  pthread_cond_t cond_p;
  int cap;
  std::queue<int> q;
  
};
