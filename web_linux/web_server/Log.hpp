#pragma once 
#include<iostream>
#include<string>
#include<sys/time.h>
using namespace std;


#define ARG_ERR 1
#define SOCK_CREAT 2
#define BIND_ERROR 3
#define LISTEN_ERROR 4

#define Normal 1
#define Warning 2
#define Error 3
#define Fatal 4


#define BACK_LOG 5 

uint64_t GetTimeStamp();
void Log(string level,string msg,string model,int number)
{
  cout << "[ " << level << " ][ " << msg << " ][ " << GetTimeStamp() << " ][ " << model << " ][ " << number << " ]" << endl;
}

#define LOG(ERR_LEVEL,MESSAGE) Log(#ERR_LEVEL,MESSAGE,__FILE__,__LINE__);

uint64_t GetTimeStamp()
{
  struct timeval _time;
  if(gettimeofday(&_time,nullptr) == 0)
  {
    return _time.tv_sec;
  }
  return 0;
}
