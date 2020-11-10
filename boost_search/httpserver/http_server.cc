#include<iostream>
#include<string>
#include"/home/pipi/ThirdPartLibForCpp/el7.x86_64/third_part/include/httpcpp/httplib.h"
#include"../searcher/searcher.h"

#define InputPath "../data/tmp/raw_input"
int main()
{
  using namespace std;
  using namespace httplib;

  searcher::Searcher searcher;
  bool ret = searcher.init(InputPath);
  if(!ret)
  {
    cout << "httpserver searcher init failed" << endl;
    return 1;
  }

  Server server;
  server.Get("/searcher",[&searcher](const Request&req,Response&resp){
      bool ret = req.has_param("query");
      if(!ret)
      {
        resp.set_content("query param miss!","text/html");
        return;
      }
      string query = req.get_param_value("query");
      string result;
      searcher.search(query,&result);
      resp.set_content(result,"application/json");
    });

  server.set_base_dir("./wwwroot");
  server.listen("0.0.0.0",8090);


  return 0;
}
