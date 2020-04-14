#include"threadPool.hpp"

int main()
{
  threadPool *tp = new threadPool(3);
  tp->InitPthreadPool();
  srand((unsigned long)time(nullptr));

  std::string s = "+-*/";
  while(true)
  {
    sleep(1);
    int x = rand()%100 + 1;
    int y = rand()%100 + 2;
    char op = s[rand() % 4 + 1];
    Task t(x,y,op);
    tp->PushTask(t);
  }
  delete tp;
  return 0;
}
