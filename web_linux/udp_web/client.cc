#include"client.hpp"
void usage()
{
  std::cout << "请输入正确的发送对象，格式为 ip + port";
}
int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    usage();
    exit(3);
  }
  client* sp = new client(argv[1],atoi(argv[2]));
  sp->InitClient();
  sp->run();
  delete sp;
  
  return 0;
}
