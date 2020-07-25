#pragma once 

#include<iostream>
#include<sstream>
#include<string>
#include<unordered_map>
using namespace std;

class Util{
  public:
    static void MakeKV(unordered_map<string,string>& kv,string &str)
    {
      size_t pos = str.find(": ");
      if(pos == string::npos)
        return ;
      string key = str.substr(0,pos);
      string val = str.substr(pos+2);
      kv.insert(make_pair(key,val));

    }
    static int StringToInt(string str)
    {
      int length;
      stringstream ss(str);
      ss >> length;
      return length;
    }
    static string SuffixToTpye(string str)
    {
      string ret;
      if(str == ".html" || str == ".htm")
        ret = "text/html";
      else if(str == ".js")
        ret = "application/x-javascript";
      else if(str == ".css")
        ret = "text/css";
      else if(str == ".jpg")
        ret = "application/x-jpg";
      else 
        ret = "text/html";

      return ret;
    }
    static string GetStatusLine(int code)
    {
      if(code == 200)
        return "HTTP/1.0 200 OK\r\n";
      else if(code == 404)
        return "HTTP/1.0 404 Not Found\r\n";
    }
    static string IntToString(int num)
    {
      stringstream ss;
      ss << num;
      return ss.str();
    }
};
