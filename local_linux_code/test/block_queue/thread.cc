#include "thread.hpp"

void *consumer(void *arg)
{
  BlockQueue *bp = (BlockQueue *)arg;

  int data;
  while(true)
  {
    bp->pop(data);
    cout << "consumer data is" << data << endl;
    sleep(1);
  }
}
void *productor(void *arg)
{
  BlockQueue *bp = (BlockQueue *)arg;
  srand((unsigned long)time(nullptr));

  while(true)
  {
    sleep(1);
    int data = rand()%100 + 1;
    bp->push(data);
    cout << "productor data is " << data << endl;
  }
}
int main()
{
  BlockQueue *bp = new BlockQueue();
  pthread_t c,p;
  pthread_create(&c,nullptr,consumer,(void *)bp);
  pthread_create(&p,nullptr,productor,(void *)bp);

  pthread_join(c,nullptr);
  pthread_join(p,nullptr);
  
  delete bp;


  return 0;
}
