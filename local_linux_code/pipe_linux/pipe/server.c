#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

#define FIFO "./myfifo"

int main()
{
    umask(0);

    if(mkfifo(FIFO,0644) < 0)
    {
        perror("mkfifo");
        return 1;
    }

    char buf[1024];
    int fd = open(FIFO,O_RDONLY);
    if(fd > 0) 
    {
        while(1)
        {

    
        ssize_t s = read(fd, buf, sizeof(buf) - 1);
        if(s > 0)
        {
            buf[s]= 0;
            printf("client->server : %s",buf);

        }else{
            printf("read error\n");
            exit(1);

        }

        }
    }
    else{
        perror("open file");
        return 1;
    }
    close(fd);
    return 0;
}
