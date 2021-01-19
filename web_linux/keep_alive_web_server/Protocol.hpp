#pragma once 

#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<string>
#include<strings.h>
#include<sstream>
#include<unordered_map>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include"Log.hpp"
#include"Util.hpp"
//#include"common.hpp"

using namespace std;

#define WWWROOT "./wwwroot"
#define WELCOME_PAGE "index.html"

#define LinkNormal 1
#define LinkError -1
#define LinkClose 0

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
      while(pos != string::npos)
      {
        string str = request_hander.substr(start,pos - start);
        Util::MakeKV(hander_kv,str);
        start = pos + 1;
        pos = request_hander.find("\n",pos + 1);
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
      cout << "path is :" << path << endl;
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
    string GetQueryString()
    {
      return query_string;
    }
    string GetRequestBody()
    {
      return request_body;
    }
    string GetPath()
    {
      return path;
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
      cout << "dubug, length: " << GetContentLength() << endl;
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
    {
      if(fd > 0)
      {
        close(fd);
        fd = -1;
      }
    }
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
    void SetResponseBody(string &str)
    {
      response_body = str;
    }
    string GetResponseBody()
    {
      return response_body;
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
    int RecvLine(string &line)
    {
      int ret = LinkNormal;
      char c = 'X';
      while(c != '\n')
      {
        ssize_t s = recv(sock,&c,1,0);
        //ssize_t s = read(sock,&c,1);
        if(s > 0)
        {
          if(c == '\r')
          {
            recv(sock,&c,1,MSG_PEEK);
            if(c == '\n')
            {
              //read(sock,&c,1);
              recv(sock,&c,1,0);
            }
            else 
            {
              c = '\n';
            }
          }
          line.push_back(c);
        }
        else if(s == 0) 
        {
          LOG(Normal,"clien close link");
          ret = LinkClose;
          break;
        }
        else 
        {
          LOG(Warning,"recv request line error");
          ret = LinkError;
          break;
        }
      }
      return ret;
      
    }
    int RecvHttpRequestLine(string &request_line)
    {
      return RecvLine(request_line);
    }
    int RecvHttpRequestHander(string &request_hander)
    {
      int flag = LinkNormal;
      string line = "";
      do{
        line = "";
      
        flag = RecvLine(line);
        if(flag == LinkNormal)
        {
          if(line != "\n")
          {
            request_hander += line;
          }
        }
        else if(flag == LinkClose || flag == LinkError)
        {
          break;
        }
      }while(line != "\n");
      return flag;
    }
    int RecvHttpRequest(HttpRequest*rq)
    {
      string request_line;
      string request_hander;
      int retLine = RecvHttpRequestLine(request_line);
      int retHander = RecvHttpRequestHander(request_hander);
      if(retLine == LinkNormal && retHander == LinkNormal)
      {
        rq->SetHttpRequestLine(request_line);
        rq->SetHttpRequestHander(request_hander);
        return 200;
      }
      else if(retLine == LinkClose || retHander == LinkClose)
      {
        return 408;
      }

      return 404;
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
      if(!rq->IsCgi())
      {
        sendfile(sock,rq->GetFd(),nullptr,rq->GetFileSize());
        //close(rq->GetFd());
      }
      else{
        string str = rsp->GetResponseBody();
        send(sock,str.c_str(),str.size(),0);
      }
    }
    Connect(int _sock):sock(_sock)
    {}
    ~Connect()
    {}

};

class Entry{
  public:
    static void MakeResponse(HttpRequest* rq,HttpResponse* rsp,int code)
    {
      string line;
      line = Util::GetStatusLine(code);

      rsp->SetHttpResponseLine(line);
      line = "Content-Type: ";
      line += Util::SuffixToTpye(rq->GetSuffix());
      cout << "debug , content-type is " << line << endl;
      line += "\r\n";
      rsp->AddHttpResponseHander(line);
      line += "Connection: keep-alive";
      line += "\r\n";
      rsp->AddHttpResponseHander(line);

      line = "Content-Length: ";
      if(!rq->IsCgi())
      {
        // no cgi
        line += Util::IntToString(rq->GetFileSize());
        line += "\r\n";
        rsp->AddHttpResponseHander(line);
        rq->OpenRescources();
      }
      else
      {
        // is cgi
        line += Util::IntToString((rsp->GetResponseBody()).size());
        line += "\r\n";
        rsp->AddHttpResponseHander(line);
      }
    }
    static int ProcessCGI(Connect*conn,HttpRequest*rq,HttpResponse*rsp)
    {
      string content_length;
      string args;
      LOG(Normal,"ProcessCgi");
      int read_pipe[2];
      int write_pipe[2];
      pipe(read_pipe);
      pipe(write_pipe);
      if(rq->IsGet())
      {
        args = rq->GetQueryString();
      }
      else if(rq->IsPost())
      {
        args = rq->GetRequestBody();
      }
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

        dup2(read_pipe[0],0);
        dup2(write_pipe[1],1);
        content_length = "Content-Length=";
        content_length += Util::IntToString(args.size());
        putenv((char *)content_length.c_str());
        string _path = rq->GetPath();
        execl(_path.c_str(),_path.c_str(),nullptr);
        
        LOG(Fatal,"execl error");
      }
      else 
      {
        //father
        close(read_pipe[0]);
        close(write_pipe[1]);
        
        cout << args << endl;
        for(auto i = 0;i < args.size();i++)
        {
          write(read_pipe[1],&args[i],1);
        }
        char c;
        string body;
        while(read(write_pipe[0],&c,1) > 0)
        {
          body.push_back(c);
        }

        rsp->SetResponseBody(body);
        pid_t ret = waitpid(id,nullptr,0);
        if(ret < 0)
        {
          LOG(Error,"watipid error");
          return 404;
        }
      }
      return 200;
    }
    static void *HanderRequest(void * args)
    {
      //混合参数
      //ThreadParameter *threadPar = (ThreadParameter *)args;
      //myEpoll *ep = threadPar->threadParameterEp;
      //int sock = threadPar->threadParameterSock;


      //单独传递sock
      int *p = (int *)args;
      int sock = *p;


      int code = 200;
      Connect *cnn = new Connect(sock);
      HttpRequest *rq = new HttpRequest();
      HttpResponse *rsp = new HttpResponse();

      code = cnn->RecvHttpRequest(rq);
      if(code == 200)
      {
        rq->RequestLinePrase();
        rq->RequestHanderPrase();
      }
      else if(code == 408)
      {
        LOG(Normal,"Client close Link");
        goto end;
      }
      else 
      {
        LOG(Error,"recv http request error");
        goto end;
      }

      if(!rq->IsMethodOk())
      {
        LOG(Warning ,"request method error");
        goto end;
        //TODO
      }
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
      //rq->Show();
      if(rq->IsCgi())
      {
        LOG(NORMAL,"exec by cgi");
        code = ProcessCGI(cnn,rq,rsp);
      }
      else{
        LOG(NORMAL,"exec by non cgi");
      }

end:
      if(code == 408)
      {
        //ep->Del(sock);
        delete cnn;
        delete rq;
        delete rsp;
        return nullptr;
      }
      MakeResponse(rq,rsp,code);
      cnn->SendResponse(rq,rsp);

      delete cnn;
      delete rq;
      delete rsp;

      return nullptr;
    }
};
