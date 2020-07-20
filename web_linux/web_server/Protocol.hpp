#pragma once 

#include<iostream>
#include<unistd.h>
#include<string>
#include<strings.h>
#include<sstream>
#include<unordered_map>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include"Log.hpp"
#include"Util.hpp"

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
    int fd;

    bool cgi;

    int file_size;
    string suffix;


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
      cout << "--------------------------------------------" << endl;
      cout << "Method is "  << method << endl;
      cout << "Url is " << url << endl;
      cout << "Version is " << version << endl;
      cout << "--------------------------------------------" << endl;
      if(url == "/")
        url += WELCOME_PAGE;
    }

    void RequestHanderPrase()
    {
      int start = 0;
      size_t pos = request_hander.find("\n");
      if(pos != string::npos)
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
    bool IsPost()
    {
      return strcasecmp(method.c_str(),"POST") == 0;
    }
    bool IsCgi()
    {
      return cgi;
    }
    bool PathIsLegal()
    {
      struct stat st;
      bool ret = true;
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
        else if((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)) 
        {
          //cgi
          cgi = true;
        }
        else{
          //
        }
        file_size = st.st_size;
        size_t pos = path.rfind('.');
        if(pos == string::npos)
        {
          suffix = ".html";
        }
        else 
        {
          suffix = path.substr(pos);
        }

      } 
      else 
      {
        //404
        ret = false;
      }

      return ret; 
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
    string GetSuffix()
    {
      return suffix;
    }
    int GetFileSize()
    {
      return file_size;
    }
    int GetFd()
    {
      return fd;
    }
    void Show()
    {
      cout << "#############################start##############################" << endl;
      cout << "debug : " << request_line;
      cout << "debug : " << request_hander; 
      cout << "debug : " << request_black;
      cout << "debug : " << request_body; 
      cout << "dubug, method: " << method << endl;
      cout << "debug, url: " << url << endl;
      cout << "debug, version: " << version << endl;
      cout << "debug, path: " << path << endl;
      cout << "debug, suffix: " << GetSuffix()  <<endl;
      cout << "debug, query_string: " << query_string << endl;
      cout << "#############################end##############################" << endl;
    }
    bool OpenRescources()
    {
      bool ret = true;
      fd = open(path.c_str(),O_RDONLY);
      if(fd < 0)
      {
        LOG(Error,"open file error");
        ret = false;
      }
      return ret;
    }
    HttpRequest():request_black("\n"),path(WWWROOT),cgi(false),fd(-1),file_size(0)
    {}

    ~HttpRequest()
    {}
};

class HttpResponse{
  private:
    string response_line;
    string response_hander;
    string response_black;
    string response_body;

  public:
      string GetResponseLine()
      {
        return response_line;
      }
      string GetResponseHander()
      {
        return response_hander;
      }
      string GetResponseBlack()
      {
        return response_black;
      }

    void SetHttpResponseLine(string &line)
    {
      response_line = line;
    }
    void AddHttpResponseHander(string &line)
    {
      response_hander += line;
    }
    HttpResponse():response_black("\n"){
    }
    ~HttpResponse()
    {}


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
    void RecvHttpRequestBody(HttpRequest *rq)
    {
      int content_length = rq->GetContentLength();
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
        rq->SetHttpRequestBody(body);
      }
      rq->SetUrlToPath();
      rq->SetCgi();
    }
    void SendResponse(HttpRequest*rq,HttpResponse*rsp)
    {
      string line = rsp->GetResponseLine();
      line += rsp->GetResponseHander();
      line += rsp->GetResponseBlack();
      
      send(sock,line.c_str(),line.size(),0);

      sendfile(sock,rq->GetFd(),nullptr,rq->GetFileSize());
    }
    Connect(int _sock):sock(_sock)
    {}
    ~Connect()
    {}

};

class Entry{
  public:
    static void MakeResponse(HttpRequest* rq,HttpResponse* rsp)
    {
      if(rq->IsCgi())
      {
        //TODO
      }
      string line = "HTTP/1.0 200 OK\r\n";
      rsp->SetHttpResponseLine(line);
      line = "Content-Type: ";
      line += Util::SuffixToTpye(rq->GetSuffix());
      line += "\r\n";
      rsp->AddHttpResponseHander(line);
      line = "Content-Length: ";
      line += Util::IntToString(rq->GetFileSize());
      line += "\r\n";
      rsp->AddHttpResponseHander(line);

      rq->OpenRescources();
    }
    static int ProcessCGI(Connect*conn,HttpRequest*rq,HttpResponse*rsp)
    {
      LOG(Normal,"ProcessCgi");
      int read_pipe[2];
      int write_pipe[2];
      pipe(read_pipe);
      pipe(write_pipe);

      pid_t id = fork();
      if(id < 0)
      {
        //
        LOG(Error,"fork error");
        return 404;
      }
      else if(id == 0)
      {
        //child
        close(read_pipe[1]);
        close(write_pipe[0]);
      }
      else 
      {
        //father
        close(read_pipe[0]);
        close(write_pipe[1]);
      }
    }
    static void ProcessNormal(Connect*conn,HttpRequest*rq,HttpResponse*rsp)
    {
      LOG(Normal,"ProcessNormal");
      MakeResponse(rq,rsp);
      conn->SendResponse(rq,rsp);
    }
    static void *HanderRequest(void * args)
    {
      int *p = (int*)args;
      int sock = *p;
      int code = 200;
      Connect *cnn = new Connect(sock);
      HttpRequest *rq = new HttpRequest();
      HttpResponse *rsp = new HttpResponse();

      cnn->RecvHttpRequest(rq);
      rq->RequestLinePrase();

      if(!rq->IsMethodOk())
      {
        LOG(Warning ,"request method error");
        goto end;
        //TODO
      }

      rq->RequestHanderPrase();

      cout << "do handerParse ok" << endl;
      if(rq->IsGet())
      {
        rq->UrlPrase();
      }
      if(rq->IsPost())
      {
        cnn->RecvHttpRequestBody(rq);
      }
      if(!rq->PathIsLegal())
      {
        code = 404;
        LOG(NORMAL,"path is not legal");
        goto end;
      }

      if(rq->IsCgi())
      {
        LOG(NORMAL,"exec by cgi");
        ProcessCGI(cnn,rq,rsp);
      }
      else{
        LOG(NORMAL,"exec by non cgi");
        ProcessNormal(cnn,rq,rsp);
      }

end:
      delete cnn;
      delete rq;
      delete rsp;

      return nullptr;
    }
};
