#include<iostream>
#include<stdio.h>
#include<unistd.h>
using namespace std;

void proBar()
{
    char a[100] = { 0  };
    int i = 0;
    for(;i<101;i++)
    {
        printf("[%-100s][%2d%%]\r",a,i);
        fflush(stdout);
        usleep(20000);
        a[i] = '=';
        if(i > 2)
        {
            a[i - 1] = '=';
            a[i]='>';
        }
    }
    cout << endl;
}
int main()
{
    proBar();

    return 0;  
}
