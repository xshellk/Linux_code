#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/shm.h>
#include<sys/ipc.h>

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
  int shmid = shmget(key,4096,IPC_CREAT);

  cout << shmid << endl;
  cout << key << endl;
  if(shmid == -1)
  {
    cout << "shmget error" << endl;
    exit(2);
  }
  

  void *str = shmat(shmid,NULL,0);
  cout << "str val is" << (signed long long)str << endl;
  if((signed long long)str < 0)
  {
    cout << "shmat error"<< endl;
    exit(3);
  }

  for(int i = 0;i < 26;i++)
  {
    ((char *)str)[i] = 'A' + i;
  }

  sleep(30);

  shmdt(str);

  shmctl(shmid,IPC_RMID,NULL);

  return 0;
}
