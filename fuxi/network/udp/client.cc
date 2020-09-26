#include"client.hpp"

using namespace std;

int main()
{
  udpClient *c = new udpClient;
  c->initClient();
  c->runClient();

  return 0;
}
