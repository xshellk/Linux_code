#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include<iostream>
using namespace std;

int main( void  )
{

  pid_t pid;
  if ( (pid=fork()) == -1  )
    perror("fork"),exit(1);
  if ( pid == 0  ){
    sleep(20);
    exit(10);

  } else {
    int st;
    int ret = wait(&st);
    if ( ret > 0 && ( st & 0X7F  ) == 0  ){ // 正常退出
      cout << WIFEXITED(st) << endl;
      cout << WEXITSTATUS(st) << endl;
    } else if( ret > 0  ) { // 异常退出
      cout << st << endl;
      //cout << WEXITSTATUS(st) << endl;
      cout << "hello" << endl;
    }

  }

}
