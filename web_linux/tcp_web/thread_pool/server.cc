#include"server.hpp"

void usage()
{
  std::cout << "请输入IP+PORT " << std::endl;
}
int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    usage();
    exit(6);
  }
  std::string ip = argv[1];
  short port = atoi(argv[2]);

  Server* sp = new Server(ip,port);
  sp->InitServer();
  sp->Start();

  return 0;
}
