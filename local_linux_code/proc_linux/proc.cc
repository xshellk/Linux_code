#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
using namespace std;

int main()
{

    char buf[20];
    int fd = open("myfile2.txt",O_RDONLY);
    
    //ssize_t s = read(fd,buf,sizeof(buf));
    if(fd > 0)
    {
      // buf[s]  = 0;
       write(1,"hellowordl",11);

    }
    //int fd = open("myfile.txt",O_WRONLY|O_CREAT,00644);
    //const char* msg = "hello world\n";
    //if(fd > 0) 
    //{
    //   write(fd,msg,strlen(msg)); 
    //}
    //else{
    //    cout << "error open file" << endl;
    //}

   // ssize_t s = read(0,buf,sizeof(buf));
   // cout << s << endl;
   // if(s > 0)
   // {
   //     buf[s] = 0;
   //     cout << (int)buf[s] << endl;
   //     write(1,buf,strlen(buf)-3);
   //     cout << endl;
   //     write(2,buf,strlen(buf));

   // }
   // 
   close(fd);
}
