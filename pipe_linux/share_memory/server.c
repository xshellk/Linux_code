#include<unistd.h>

#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define PATH_NAME "/tmp"
#define PROJ_ID 0x6666 
int main()
{
    int ret;
    key_t key = ftok(PATH_NAME,PROJ_ID);
    if(key < 0)
    {
        perror("ftok");
        ret = 1;
        return ret;
    }
    int shm_id = shmget(key,4096,IPC_CREAT|IPC_EXCL);
    if(shm_id == -1)
    {
        perror("shmget");
        ret = 2;
        return ret;
    }
    void *str = shmat(shm_id,NULL,0);
    if((signed long long )str < 0)
    {
        perror("shmat"); 
        ret = 3;
        return ret;
    }
    int i = 0;
    sleep(3);
    for(i = 0;i < 26; i++)
    {
        printf("cilent -> server:%s\n",(char *)str);
        sleep(1);
    }
    shmdt(str);
    shmctl(shm_id,IPC_RMID,NULL);

}
