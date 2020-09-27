#include"server.hpp"

using namespace std;

void fun()
{
  cout << "Please input IP+PORT" << endl;
}

int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    fun();
  }
  tcpServer *s = new tcpServer(argv[1],atoi(argv[2]));
  s->initServer();
  s->runServer();

  delete(s);
  return 0;
}
