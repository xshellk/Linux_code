#include"server.hpp"
void usage()
{
  std::cout << "请输入ip + port" << std::endl;
}
int main(int argc,char *argv[])
{
  if(argc != 3)
  {
    usage();
    exit(2);
  }
  server *sp = new server(argv[1],atoi(argv[2]));
  sp->InitServer();
  sp->run();
  delete sp;

  return 0;
}

