#pragma once 

#include<iostream>
#include<string>
#include<unordered_map>
using namespace std;

class Util{
  public:
    static void MakeKV(unordered_map<string,string>& kv,string &str)
    {
      ssize_t pos = str.find(": ");
      if(pos == string::npos)
        return ;
      string key = str.substr(0,pos);
      string val = str.substr(pos+2);
      kv.insert(make_pair(key,val));

    }
};
