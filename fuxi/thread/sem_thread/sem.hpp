#pragma once 

#include<iostream>
#include<pthread.h>
#include<vector>
#include<semaphore.h>
#include<unistd.h>

class ringQueue{
public:
  ringQueue(int _cap = 10):cap(_cap),consumer_stap(0),productor_step(0)
  {
    sem_init(&sem_data,0,cap);
    sem_init(&sem_space,0,cap);
    q.resize(cap);
  }
  ~ringQueue()
  {
    sem_destroy(&sem_data);
    sem_destroy(&sem_space);
  }
  void P(sem_t *sem)
  {
    sem_wait(sem);
  }
  void V(sem_t *sem)
  {
    sem_post(sem);
  }

  void putDate(int data)
  {
    P(&sem_space);
    productor_step++;
    productor_step %= cap;
    q[productor_step] = data;
    V(&sem_data);
  }
  void getDate(int *data)
  {
    P(&sem_data);
    *data = q[consumer_stap];
    consumer_stap++;
    consumer_stap %= cap;
    V(&sem_space);
  }

private:
  sem_t sem_data;
  sem_t sem_space;
  int consumer_stap;
  int productor_step;
  int cap;
  std::vector<int> q;

};
