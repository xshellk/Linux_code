#include"mutex.hpp"
#include<unistd.h>

using namespace std;
void *consumer(void *store)
{
  blockQueue *con = (blockQueue *)store;
  int ret;
  while(true)
  {
    con->pop(&ret);
    cout << "consumer get : " << ret << endl;
    sleep(1);
  }
   return NULL;
}
void *productor(void *store)
{
  blockQueue *pro = (blockQueue *)store;
  srand((unsigned long)time(nullptr));
  while(true)
  {
    pro->push(rand() % 100 + 1);
    sleep(1);
  }

  return NULL;
}

int main()
{
  blockQueue fac;
  cout << "consumer with productor begin :" << endl;
  pthread_t con;
  pthread_t pro;
  pthread_create(&con,NULL,consumer,&fac);
  pthread_create(&pro,NULL,productor,&fac);


  pthread_join(con,NULL);
  pthread_join(pro,NULL);

  return 0;
}
