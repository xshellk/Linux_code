#include"select.hpp"

using namespace std;


int main()
{
  selectServer *p = new selectServer;
  p->selectInit();
  p->start();


  delete(p);

  return 0;
}
