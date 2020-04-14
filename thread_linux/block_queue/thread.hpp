#pragma once 

#include<iostream>
#include<stdlib.h>
#include<unistd.h>

#include<queue>
#include<pthread.h>

using namespace std;

class BlockQueue{
    private:
        queue<int> q;
        size_t cap;

        pthread_mutex_t lock;
        pthread_cond_t cond_c;
        pthread_cond_t cond_p;
    private:
        bool isFull()
        {
            return q.size() == cap;
        }
        bool isEmpty()
        {
            return q.size() == 0;
        }
        void LockQueue()
        {
            pthread_mutex_lock(&lock);
        }
        void UnLockQueue()
        {
            pthread_mutex_unlock(&lock);
        }
public:
    void push(int i)
    {
        LockQueue();
        while(isFull())
        {
            pthread_cond_wait(&cond_p,&lock);
        }
        q.push(i);
        UnLockQueue();
    }
    void pop(int &data)
    {
        LockQueue();
        while(isEmpty())
        {
            pthread_cond_wait(&cond_c,&lock);
        }
        data = q.front();
        q.pop();
        UnLockQueue();
        pthread_cond_signal(&cond_p);
    }
    int front()
    {
        return q.front();
    }

    BlockQueue(int _cap = 20) : cap(_cap)
    {
        pthread_mutex_init(&lock,nullptr);
        pthread_cond_init(&cond_c,nullptr);
        pthread_cond_init(&cond_p,nullptr);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&cond_c);
        pthread_cond_destroy(&cond_p);
    }

};
