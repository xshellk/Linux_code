#include"pool_thread.hpp"

using namespace std;

int main()
{
  threadPool thp;
  thp.pthreadInit();
  while(true)
  {
    Task t;
    thp.pushTask(t);
    sleep(1);
  }

  return 0;
}
