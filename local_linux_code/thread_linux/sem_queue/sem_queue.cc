#include"sem_queue.hpp"

void *consume(void * arg)
{
  RingQueue *qu = (RingQueue*)arg;
  while(true)
  {
    sleep(1);
    int data = 0;
    qu->pop(data);
    std::cout<< "this is consume data" << data << std::endl;
  }
}
void *product(void * arg)
{
  RingQueue *qu = (RingQueue *)arg;

  srand((unsigned long)time(nullptr));
  while(true)
  {
    sleep(1);
    int data = rand() % 100 + 1;
    qu->push(data);
    std::cout << "this is product data " << data << std::endl;
  }
}
int main()
{
  RingQueue *qu = new RingQueue();

  pthread_t c,p;
  pthread_create(&c,nullptr,consume,(void *)qu);
  pthread_create(&p,nullptr,product,(void *)qu);

  pthread_join(c,nullptr);
  pthread_join(p,nullptr);

  delete qu;
  return 0;
}
