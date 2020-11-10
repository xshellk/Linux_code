#pragma once 

#include<iostream>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<vector>


class RingQueue{
private:
  std::vector<int> ring;
  sem_t sem_data;
  sem_t sem_black;

  int product_step;
  int consume_step;
  int cap;

private:
  void P(sem_t &sem)
  {
    sem_wait(&sem);
  }
  void V(sem_t &sem)
  {
    sem_post(&sem);
  }
public:
  void push(int &data)
  {
    P(sem_black);
    ring[product_step] = data;
    V(sem_data);
    product_step++;
    product_step %= cap;
  }
  void pop(int &data)
  {
    P(sem_data);
    data = ring[consume_step];
    V(sem_black);
    consume_step++;
    consume_step %= cap;
  }
public:
  RingQueue(int _cap = 20) : cap(_cap),ring(_cap)
  {
    product_step = consume_step = 0;
    sem_init(&sem_black,0,cap);
    sem_init(&sem_data,0,0);
  }
  ~RingQueue()
  {
    sem_destroy(&sem_black);
    sem_destroy(&sem_data);
  }
};
