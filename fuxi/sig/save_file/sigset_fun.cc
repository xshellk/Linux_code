#include<iostream>
#include<signal.h>
#include<unistd.h>

using namespace std;

int main()
{
  sigset_t s,old_s;
  sigemptyset(&s);
  sigemptyset(&old_s);

  sigaddset(&s,2);

  if(sigismember(&s,SIGINT))
  {
    cout << "sigaddset success" << endl;
  }
  
  if(0 != sigprocmask(SIG_BLOCK,&s,&old_s))
  {
    cout << "sigprocmask error" << endl;
  }
  cout << "SIG_INT is block" << endl;


  for(int i = 0;;i++)
  {
    if(i == 4)
    {
      //sigdelset(&s,2);
      cout << "SIG_INT is free" << endl;
      sigprocmask(SIG_SETMASK,&old_s,NULL);
    }
    sleep(1);
  }


  return 0;
}
