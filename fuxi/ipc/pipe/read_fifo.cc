#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string>


using namespace std;


int main()
{

  int fd = open("pipo",O_RDONLY);

  if(fd < 0)
  {
    cout << "open pipr ipc to read error" << endl;
    exit(1);
  }
  char buf[100];
  read(fd,buf,40);
  cout << buf << endl;

  close(fd);


  return 0;
}
