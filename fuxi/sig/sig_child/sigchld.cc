#include<iostream>
#include<signal.h>
#include<unistd.h>

using namespace std;

void handler(int sig)
{
  cout << "this sig is #" << sig << endl;
}
int main()
{
  pid_t id = fork();

  if(id > 0)
  {
    //father
    signal(SIGCHLD,handler);
    cout << "done signal fun" << endl;

    sleep(3);
    
  }
  else if(id == 0)
  {
    //child
    kill(getpid(),2);
  }
  else{
    //error
    perror("fork");
  }

  return 0;
}
