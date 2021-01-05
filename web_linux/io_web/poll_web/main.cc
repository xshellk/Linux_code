#include<iostream>
#include<unistd.h>
#include<poll.h>

using namespace std;

int main()
{
  struct pollfd pofd[5];
  pofd[0].fd = 0;
  pofd[0].events = POLLIN;

  int ret = poll(pofd,1,10000);

  if(ret == 0)
  {
    cout << "time out"<< endl;
  }
  else if (ret > 0){
    if(pofd[0].revents == POLLIN)
    {
      char buf[1024];
      read(0,buf,1024);
      cout << "get inf :" << buf << endl;
    }
    else 
    {
      cout << "don't POLLOUT" << endl;
    }
  }
  else 
  {
    cout << "poll error" << endl;
  }
  
  return 0;
}
