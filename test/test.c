#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    pid_t id = fork();
    if(id > 0)
    {
        printf("this is father process\n");
        sleep(4);
        exit(1);
    }
    else if( id == 0 )
    {
        while(1)
        {
        printf("this is child process\n");
        sleep(1);
        }
    }
    else 
    {
        perror("fork");
    }
    return 0;
}
