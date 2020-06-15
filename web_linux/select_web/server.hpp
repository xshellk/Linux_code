#pragma once 

#include<iostream>
#include<vector>
#include<string>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<sys/select.h>

#define MAX sizeof(fd_set)*8

using namespace std;

class selectServer{
private:
  int port;
  int listen_sock;
  vector<int> vfds; 
public:
  selectServer(int _port) : port(_port),listen_sock(-1),vfds(MAX,-1) {}
  selectServer() : port(8080),listen_sock(8080),vfds(MAX,-1) {}

  void initServer()
  {
    //struct sockadd_in local;
    struct sockaddr_in local;
    bzero(&local,sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    listen_sock = socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock < 0)
    {
      cerr << "socker error" << endl;
      exit(2);
    }
    if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local)) < 0)
    {
      cerr << "bind error" << endl;
      exit(3);
    }
    if(listen(listen_sock,5) < 0)
    {
      cerr << "listen error" << endl;
      exit(4);
    }
    int opt = 1;
    setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

  }

  void handerEvent(fd_set *rfds)
  {
    for(int i = 0;i < MAX;i++)
    {
      if(vfds[i] < 0)
        continue;

      if(FD_ISSET(vfds[i],rfds))
      {
        if(vfds[i] == listen_sock)
        {
          struct sockaddr_in peer;
          socklen_t len = sizeof(peer);
          int sock = accept(listen_sock,(struct sockaddr*)&peer,&len);
          if(sock < 0)
          {
            cerr << "accept error " << endl;
            continue;
          }
          cout << "get a new link :"<< sock << endl;
          int j = 0;
          for(;j < MAX;j++)
          {
            if(vfds[j] < 0)
              break;
          }
          if(j == MAX)
          {
            cout << "server full" << endl;
            close(sock);
          }
          else{
            vfds[j] = sock;
          }

        }
        else 
        {
          char buf[1024];
          ssize_t s = recv(vfds[i],buf,sizeof(buf) - 1,0);
          if(s > 0)
          {
            buf[s] = 0;
            cout << "client #:" << buf <<endl;
          }
          else 
          {
            if(s == 0)
              cout << "peer link close,server close too" << endl;
            else 
              cout << "recv error!,server close" <<endl;
            close(vfds[i]);
            vfds[i] = -1;
          }
        }

      }
      else 
      {
        cout << vfds[i] << ": curr fd is not readly" << endl;
      }
    }
  }
  void start()
  {
    vfds[0] = listen_sock;
    fd_set rfds;

    for(;;)
    {
      int maxfd = listen_sock;
      FD_ZERO(&rfds);
      for(auto it = vfds.begin();it != vfds.end();++it)
      {
        if(*it < 0)
          continue;
        FD_SET(*it,&rfds);
        if(maxfd < *it)
          maxfd = *it;
      }
      cout << "maxfd: " << maxfd << endl;
      
      switch(select(maxfd+1,&rfds,nullptr,nullptr,nullptr))
      {
        case 0:
          cout << "time out..." << endl;
          break;
        case -1:
          cerr << "select error" << endl;
          break;
        default:
          cout << "get a event" << endl;
          handerEvent(&rfds);
          break;

      }
    }


  }

};
