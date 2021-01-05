#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string>
#include<sys/wait.h>

using namespace std;
//block
//
int main()
{
  
  int fl = fcntl(0,F_GETFL);
  if(fl < 0)
  {
    cout << "getfl error" <<endl;
    exit(2);
  }
  fcntl(0,F_SETFL,fl|O_NONBLOCK);
  char buf[1024];

  while(1)
  {
    sleep(1);
    read(0,&buf,sizeof(buf)/sizeof(buf[0]));

    cout << buf << endl;
  }

}
//
//noblock
