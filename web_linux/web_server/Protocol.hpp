#pragma once 

#include<iostream>
#include<string>
#include<strings.h>
#include<sstream>
#include<sys/types.h>
#include<sys/socket.h>
#include"Log.hpp"
#include<unordered_map>

using namespace std;


class HttpRequest{
  private:
    string request_line;
    string request_hander;
    string request_black;
    string request_text;

  private:
    string method;
    string url;
    string version;

    unordered_map<string,string> hander_kv;

  public:
    void SetHttpRequestLine(string &line)
    {request_line = line;}
    void SetHttpRequestHander(string &hander)
    {request_hander = hander;}

    void RequestLinePrase()
    {
      stringstream ss(request_line);
      ss >> method >> url >> version;
      cout << "method is "  << method << endl;
      cout << "url is " << url << endl;
      cout << "version is " << version << endl;
    }
    void RequestHanderPrase()
    {
      int start = 0;
      ssize_t pos = request_hander.find("\n");
      while(pos != string::npos)
      {
        string str = request_hander.substr(start,pos - start);
        
      }
    }

    bool IsMethodOk()
    {
      if(strcasecmp(method.c_str(),"GET") == 0 || strcasecmp(method.c_str(),"POST") == 0)
        return true;
      else 
        return false;
    }
    bool IsGet()
    {
      return strcasecmp(method.c_str(),"GET") == 0;
    }
    HttpRequest():request_black("\n")
    {}

    ~HttpRequest()
    {}
};

class HttpResponse{
  private:
    string response_line;
    string response_hander;
    string response_black;
    string response_text;

};

class Connect{
  private:
    int sock;
  public:
    void RecvLine(string &line)
    {
      char c = 'X' ;
      while(c != '\n')
      {
        ssize_t s = recv(sock,&c,1,0);
        if(s > 0)
        {
          if(c == '\r')
          {
            recv(sock,&c,1,MSG_PEEK);
            if(c == '\n')
            {
              recv(sock,&c,1,0);
            }
            else 
            {
              c = '\n';
            }
          }
          line.push_back(c);
        }
        else 
        {
          LOG(Warning,"recv request line error");
          break;
        }
      }
      
    }
    void RecvHttpRequestLine(string &request_line)
    {
      RecvLine(request_line);
    }
    void RecvHttpRequestHander(string &request_hander)
    {
      string line = "";
      do{
        line = "";
        RecvLine(line);
        if(line != "\n")
          request_hander += line;
      }while(line != "\n");
    }
    void RecvHttpRequest(HttpRequest*rq)
    {
      string request_line;
      string request_hander;
      RecvHttpRequestLine(request_line);
      RecvHttpRequestHander(request_hander);
      rq->SetHttpRequestLine(request_line);
      rq->SetHttpRequestHander(request_hander);
    }

    Connect(int _sock):sock(_sock)
    {}
    ~Connect()
    {}

};

class Entry{
  public:
    static void *HanderRequest(void * args)
    {
      int sock = (int)*args;
      Connect *cnn = new Connect(sock);
      HttpRequest *rq = new HttpRequest();
      HttpResponse *rsp = new HttpResponse();

      cnn->RecvHttpRequest(rq);
      rq->RequestLinePrase();

      if(!rq->IsMethodOk())
      {
        LOG(Warning ,"request method error");
        //TODO
      }

      rq->RequestHanderPrase();

      if(rq->IsGet())
      {

      }
      else{

      }


      delete cnn;
      delete rq;
      delete rsp;
      
    }
};
