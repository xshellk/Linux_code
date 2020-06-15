#include"server.hpp"
void usage(string proc)
{
  cout << "usage :" << proc << " port " << endl;
}
int main(int argc,char *argv[])
{
  if(argc != 2)
  {
    usage(argv[0]);
    exit(1);
  }
  selectServer *ss = new selectServer(atoi(argv[1]));
  ss->initServer();
  ss->start();

  return 0;
}
