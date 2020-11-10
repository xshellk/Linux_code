#pragma once 

#include<iostream>

using namespace std;

class IoFile
{
  public:
    IoFile(int _port):listen_sock(0),port(_port)
    {}
    ~IoFile()
    {}

  public:
      int listen_sock;
      int port;
};
