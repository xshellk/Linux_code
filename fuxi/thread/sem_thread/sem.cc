#include"sem.hpp"

void *consumer(void *sem)
{
  std::cout << "begin do consumer fun" << std::endl;
  ringQueue* con = (ringQueue*)sem;
  int val;
  while(1)
  {
    sleep(1);
    con->getDate(&val);
    std::cout << "get val is " << val << std::endl;

  }
  return NULL;
}
void *productor(void *sem)
{
  std::cout << "begin do productor fun" << std::endl;
  ringQueue *pro = (ringQueue *)sem;
  srand((unsigned long)time(nullptr));
  while(1)
  {
    sleep(1);
    pro->putDate(rand()%100 + 1);
  }
  return NULL;
}

int main()
{
  ringQueue sem;
  std::cout << "done create sem" << std::endl;

  pthread_t con;
  pthread_t pro;

  pthread_create(&pro,NULL,productor,&sem);
  pthread_create(&con,NULL,consumer,&sem);


  pthread_join(con,NULL);
  pthread_join(pro,NULL);


  return 0;
}
