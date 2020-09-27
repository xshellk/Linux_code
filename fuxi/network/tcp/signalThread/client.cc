#include"client.hpp"

using namespace std;

int main()
{
  tcpClient *c = new tcpClient;
  c->initClient();
  c->connect();
  c->runClient();

  delete(c);

  return 0;
}
