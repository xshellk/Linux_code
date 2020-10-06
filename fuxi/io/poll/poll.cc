#include<iostream>
#include<poll.h>
#include<unistd.h>


using namespace std;

int main()
{
  struct pollfd fd;
  fd.fd = 0;
  fd.events = POLLIN;
  for(;;)
  {
    int ret = poll(&fd,1,0);
    if(ret > 0)
    {
      // ret > 0, ok
      if(fd.revents == POLLIN)
      {
        char buf[1024];
        ssize_t s = read(fd.fd,buf,1023);
        if(s < 0)
        {
          cout << "read error" << endl;
        }
        else 
        {
          cout << "get information #" << buf << endl;
        }
      }
    }
    else if(ret == 0)
    {
      // ret == 0, timeout
    }
    else 
    {
      // ret < 0, error
    }
  }



  return 0;
}
