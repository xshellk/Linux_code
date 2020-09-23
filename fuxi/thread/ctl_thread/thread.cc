#include<iostream>
#include<pthread.h>
#include<unistd.h>

using namespace std;

void * handler(void * tmp)
{
  int num = *(int *)tmp;
  void *ret = NULL;
  ret = new int(10);
  //分别执行三种不同的进程推迟，根据参数
  if(num == 1)
  {
    sleep(1);
    cout << "do exit fun" << endl;
    cout << "pthread_self is " << pthread_self() << endl;
    void *p = malloc(2*sizeof(int));
    *(int *)p = 10;

    pthread_exit(p);

  }
  else if(num == 2) 
  {
    cout << "thread will do cancel;" <<endl;
    sleep(20);
  }

  return ret;
}

int main()
{

  //创建进程
  pthread_t tid;
  void *ret_val = NULL;
  int num;
  cout << "get cin >> ";
  cin >> num;

  pthread_create(&tid,NULL,handler,&num);

  if(num == 1)
  {
    //pthrad_exit

    cout << "doing pthread_exit fun" << endl;
    int ret = 0;
    //pthread_join(tid,&ret_val);
    pthread_detach(tid);
    cout << "do exit fun ret is " << ret << endl;
    cout << "this ret_val point is " << ret_val << endl;
    cout << "do pthrad exit fun ret val is " << *(int *)ret_val << endl;
  }
  else if(num == 2)
  {
    //pthread_cancel
    cout << "father will doing pthread_cancel" << endl;
    pthread_cancel(tid);
    pthread_join(tid,&ret_val);
  
    cout << "do cancel fun ret num is" << ret_val << endl;
  }
  else 
  {
    //normal return 
    
    cout << "don't do anything, normal return " << endl;
    pthread_join(tid,&ret_val);
    cout << "return val is " << ret_val;
  }

  return 0;
}
