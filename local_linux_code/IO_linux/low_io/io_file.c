#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>

int main()
{
    char buf[100];
    int fd_wr = open("hello.txt",O_WRONLY|O_CREAT,0644);
    char *msg = "hello world\n";
    if(fd_wr < 0)
    {
        perror("open file");
    }
    write(fd_wr,msg,strlen(msg));
    close(fd_wr);
    int fd_rd = open("hello.txt",O_RDONLY); 
    ssize_t s = read(fd_rd,buf,sizeof(buf));
    buf[s] = '\0';
    printf("%s",buf);

    return 0;
}
