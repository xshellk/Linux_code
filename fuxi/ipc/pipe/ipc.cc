#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<string>


using namespace std;


int main()
{
  int ret = mkfifo("pipo",0644);
  if(ret < 0)
  {
    cout << "mkfifo error" << endl;
    exit(2);
  }

  int fd = open("pipo",O_WRONLY);

  if(fd < 0)
  {
    cout << "open pipr ipc error" << endl;
    exit(1);
  }
  string str = "helloworld";
  write(fd,str.c_str(),str.size());

  close(fd);


  return 0;
}
