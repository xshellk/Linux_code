#include<iostream>
#include<stdlib.h>
#include<string>
#include<unistd.h>
#include"Util.hpp"
#include"Log.hpp"
using namespace std;

int main()
{
  char *content_length = getenv("Content-Length");
  if(nullptr != content_length)
  {
    string str = content_length;
    int cl = Util::StringToInt(str);
    char c;
    string args;
    for(auto i = 0;i < cl;i++)
    {
      read(0,&c,1);
      args.push_back(c);
    }
    string out;
    out = "<html>\r\n<body>\r\n<h1>hello world</h1>\r\n<p>hello world</p>\r\n</body>\r\n</html>";
    cout << out << endl;
  }
  else{
    LOG(Error,"getenv error");
  }
  return 0;
}
