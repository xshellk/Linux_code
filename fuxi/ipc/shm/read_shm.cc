#include<iostream>
#include<sys/stat.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<string>
#include<unistd.h>

using namespace std;

#define PATH_NAME "./tmp"
#define PROJ_ID 0x6666

int main()
{
  int key = ftok(PATH_NAME,PROJ_ID);
  if(key < 0)
  {
    cout << "ftok error" << endl;
    exit(1);
  }
  int shm_id = shmget(key,4096,IPC_CREAT);
  if(shm_id < 0)
  {
    cout << "shmget error" << endl;
    exit(2);
  }

  void *str = shmat(shm_id,NULL,0);
  if((signed long long)str < 0)
  {
    cout << "shmat error"<< endl;
    exit(3);
  }

  string buf = (char *)str;

  cout << buf << endl;
  sleep(3);

  return 0;
}
