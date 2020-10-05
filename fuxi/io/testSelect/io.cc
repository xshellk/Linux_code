#include<iostream>
#include<sys/select.h>
#include<unistd.h>
#include<stdio.h>

using namespace std;

int main()
{
  fd_set fd;
  FD_ZERO(&fd);
  FD_SET(0,&fd);
  string str;
  
  for(;;)
  {
    cout << "Please input > ";
    fflush(stdout);
    int ret = select(2,&fd,NULL,NULL,NULL);
    cin >> str;
    if(ret < 0)
    {
      cout << "select error" << endl;
      exit(2);
    }else if(ret == 0)
    {
      cout << "don't get anything" << endl;
    }

    if(FD_ISSET(0,&fd))
    {
      cout << "output is > "  << str << endl;
    }
    FD_ZERO(&fd);
    FD_SET(0,&fd);
  }
  return 0;
}
