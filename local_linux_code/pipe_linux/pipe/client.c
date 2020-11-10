#include<stdio.h>
#include<string.h>

#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

#define FIFO "./myfifo"

int main()
{
    char buf[1024];
    int fd = open(FIFO,O_WRONLY);
    if(fd < 0) 
    {
        perror("open file");
    }

    while(1)
    {
        printf("Please Enter# ");
        fflush(stdout);
        ssize_t s = read(0,buf,sizeof(buf) - 1);
        if(s > 0)
        {
            buf[s] = 0;
            write(fd,buf,s);
        }
        else{
            break;
        }
    }


    close(fd);
    return 0;
}
