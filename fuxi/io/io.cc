#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


using namespace std;


int main()
{
  int fd = open("test.txt",O_WRONLY|O_CREAT,0644);
  if(fd < 0)
  {
    cout << "open file error" << endl;
    exit(1);
  }
  dup2(1,fd);
  write(fd,"helloworld",11);
  //此时虽然是想fd中输出，但实际输出到了屏幕(标准输出)中
  close(fd);
  return 0;
}
