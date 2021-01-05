#pragma once 

#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string>
#include<sys/wait.h>

//block
//
int main()
{
  pid_t pid = fork();
  int fd = open("log.txt",O_RDWR|O_CREAT,0644);
  if(fd < 0)
  {
    perror("open file error");
    exit(1);
  }

  if(pid == 0)
  {
    //child
    sleep(5);
    std::string str("hello world\n");
    write(fd,str.c_str(),str.size());
    std::cout << "write success" << std::endl;
  }
  else if(pid > 0)
  {
    char buf[50];
    ssize_t s = read(fd,&buf,50);
    if(s < 0)
    {
      std::cout << "read error" << std::endl;
      goto end;
    }
    
end:
    waitpid(pid,NULL,0);

  }
  else 
  {

  }

}
//
//noblock
