#include"client.hpp"
void usage()
{
  std::cout << "请输入正确的个格式 ip+port" << std::endl;
  exit(4);
}
int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    usage();
  }
  std::string ip = argv[1];
  short port = atoi(argv[2]);

  client *sp = new client(ip,port);
  sp->InitClient();
  sp->Connect();
  sp->run();


  return 0;
}
