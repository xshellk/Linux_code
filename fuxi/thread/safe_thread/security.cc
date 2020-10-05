#include<iostream>
#include<pthread.h>

using namespace std;

template<class Type>
class hungrySingleton{
private:
  static Type data;

public:
  static Type *getVal()
  {
    return &data;
  }
};

template<class Type>
class lazySingleton{
private:
  static volatile Type *data;
  static pthread_mutex_t lock;
public:
  static Type * getVal()
  {
    if(data == NULL)
    {
      pthread_mutex_lock(&lock);
      if(data == NULL)
      {
        data = new Type;
      }
      pthread_mutex_unlock(&lock);
    }
    
    return data;
  }
};

int main()
{


  return 0;
}
