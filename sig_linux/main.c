#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int main()
{
    int i = 0;
    sigset_t set;
    sigset_t oset;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);
    sigprocmask(SIG_BLOCK,&set,&oset);
    while(1)
    {
        i++;
        printf("hello world\n");
        sleep(1);
        if(i >= 10)
        {
            sigprocmask(SIG_UNBLOCK,&set,NULL);
        }
    }
    return 0;

}
