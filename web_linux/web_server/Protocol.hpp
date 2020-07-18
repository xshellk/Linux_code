#pragma once 

#include<iostream>
#include<string>
#include<strings.h>
#include<sstream>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include"Log.hpp"
#include"Util.hpp"
#include<unordered_map>

using namespace std;

#define WWWROOT "./wwwroot"
#define WELCOME_PAGE "index.html"

class HttpRequest{
  private:
    string request_line;
    string request_hander;
    string request_black;
    string request_body;

  private:
    string method;
    string url;
    string version;

    unordered_map<string,string> hander_kv;

    string path;
    string query_string;

    bool cgi;


  public:
    void SetHttpRequestLine(string &line)
    {request_line = line;}
    void SetHttpRequestHander(string &hander)
    {request_hander = hander;}
    void SetHttpRequestBody(string &body)
    {request_body = body;}
    void SetUrlToPath()
    {path = url;}
    void SetCgi()
    {cgi = true;}

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
      size_t pos = request_hander.find("\n");
      while(pos != string::npos)
      {
        string str = request_hander.substr(start,pos - start);
      }
    }
    void UrlPrase()
    {
      size_t pos = url.find("?");
      if(pos != string::npos)
      {
        path += url.substr(0,pos);
        query_string += url.substr(pos+1);

        cgi = true;
      }
      else 
      {
        path += url;
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
    bool PathIsLegal()
    {
      struct stat st;
      if(stat(path.c_str(),&st) == 0)
      {
        if(S_ISDIR(st.st_mode))
        {
          if(path[path.size() - 1] != '/')
          {
            path+='/';
          }
          path += WELCOME_PAGE;
        }
        else 
        {
          //cgi
        }

      } 
      else 
      {
        //404
      }
      
    }

    

    int GetContentLength()
    {
      auto it = hander_kv.find("Content-Length");
      if(it == hander_kv.end())
      {
        LOG(NORMAL_ERR,"post method,but no request_body");
        return NORMAL_ERR;
      }
      return Util::StringToInt(it->second);
    }
    void Show()
    {
      cout << "#############################start##############################";
      cout << "debug : " << request_line;
      cout << "debug : " << request_hander; 
      cout << "debug : " << request_black;
      cout << "debug : " << request_body; 
      cout << "dubug, method: " << method << endl;
      cout << "debug, url: " << url << endl;
      cout << "debug, version: " << version << endl;
      cout << "debug, path: " << path << endl;
      cout << "debug, query_string: " << query_string << endl;
      cout << "#############################start##############################";
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
    void RecvHttpRequestBody(HttpRequest &rq)
    {
      int content_length = rq.GetContentLength();
      if(content_length > 0)
      {
        string body;
        char c;
        while(content_length > 0)
        {
          recv(sock,&c,1,0);
          body.push_back(c);
          content_length--;
        }
        rq.SetHttpRequestBody(body);
      }
      rq.SetUrlToPath();
      rq.SetCgi();
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
        rq->UrlPrase();
      }
      else{
        cnn->RecvHttpRequestBody(rq);
      }

      if(rq->PathIsLegal())
      {

      }


      delete cnn;
      delete rq;
      delete rsp;
      
    }
};
