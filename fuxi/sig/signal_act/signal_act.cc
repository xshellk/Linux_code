#include<iostream>
#include<signal.h>
#include<unistd.h>

using namespace std;


int main()
{
  signal(2,SIG_IGN);

  for(int i = 0;;i++)
  {
    if(i == 4)
    {
      cout << "will do release sig 2" << endl;;
      signal(2,SIG_DFL);
      //sigset_t s;
      //sigemptyset(&s);
      //sigaddset(&s,2);
      
    }
    sleep(1);
  }

  return 0;
}
