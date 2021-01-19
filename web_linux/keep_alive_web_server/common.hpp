#pragma once 

#include"io.hpp"

class ThreadParameter
{
public:
  int threadParameterSock;
  myEpoll *threadParameterEp;
  ThreadParameter()
  {}
  ~ThreadParameter()
  {}
};
