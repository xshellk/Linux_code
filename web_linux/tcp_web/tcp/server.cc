#include"server.hpp"
void usage()
{
  std::cout << "请输入 ip+port格式" << std::endl;
}
int main(int argc,char* argv[])
{
  if(argc != 3)
  {
    usage();
    exit(5);
  }
  std::string ip = argv[1];
  short port = atoi(argv[2]);
  server *p = new server(ip,port);
  p->InitServer();
  p->Run();

  delete p;
  return 0;
}
