#include<stdio.h>
#include<unistd.h>

#include<pthread.h>
void *thread_fun(void *str)
{
    char *msg = (char *)str;
    while(1)
    {
        printf("%s\n",msg);
        sleep(1);
    }
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,thread_fun,(void*)"hello world");
    while(1)
    {
        printf("this is father thread\n");
        sleep(1);
    }
    return 0;
}
