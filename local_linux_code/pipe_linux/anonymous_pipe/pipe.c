#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<error.h>

int main()
{
    int fd[2];
    if(pipe(fd) != 0)
    {
        perror("pipe create");
    }
    int id = fork();
    if(id > 0)
    {
        char buf[100];
        close(fd[1]);
        sleep(5);
        ssize_t s = read(fd[0],buf,sizeof(buf)); 
        if(s < 0)
        {
            perror("read");
        }
        buf[s] = 0;
        printf("%s\n",buf);
    }
    else if( id == 0 )
    {
        const char *msg = "hello world\n";
        close(fd[0]);
        while(1)
        {
            write(fd[1],msg,strlen(msg));
            sleep(1);
        }
    }
    else 
    {
        perror("fork create");
    }
    return 0;
}
