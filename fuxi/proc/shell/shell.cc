#include<iostream>
#include<string>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

using namespace std;


int main()
{
  while (1)
  {
    char *arr[20] = { 0  };
    string tmp[20];
    string str;
    cout << "get cin >> " << endl;
    getline(cin, str);
    size_t flag = 0;
    int count = 0;


    // 转化
    
    while (flag < str.size())
    {
      if (str.find(' ', flag) != string::npos)
      {
        tmp[count] = str.substr(flag, str.find(' ', flag) - flag);
        arr[count] = const_cast<char*>(tmp[count].c_str());
      }
      else
      {
        tmp[count] = str.substr(flag, str.size() - flag);
        arr[count] = const_cast<char *>(tmp[count].c_str());
        break;
      }
      flag = str.find(' ', flag) + 1;
      count++;
    }

    //创建子进程

    pid_t id = fork();
    if (id < 0)
    {
      cout << "fork error" << endl;
      exit(1);

    }
    else if (id == 0)
    {
      //child
      int ret = execvp(arr[0], arr);
      if (ret < 0)
      {
        cout << "exec fun error" << endl;
        return 2;
      }
    }
    else
    {
      //father
      int status;
      waitpid(id, &status, 0);

    }



  }

  return 0;
}

