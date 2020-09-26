#include"server.hpp"
#include<stdlib.h>

using namespace std;
void fun()
{
  cout << "Plase input IP PORT" << endl;
}

int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    fun();
    exit(5);
  }
  udpServer *s = new udpServer(argv[1],atoi(argv[2]));
  s->initServer();
  s->runServer();

  return 0;
}
