#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include<boost/filesystem/path.hpp>
#include<boost/filesystem/operations.hpp>

#include"../common/Util.hpp"


using namespace std;

string g_input_path = "../data/input";
string g_output_path = "../data/tmp/raw_input";

class DocInfo{
public:

  string title;
  string url;
  string content;
};
bool EnumFile(const string & input,vector<string> *output)
{
  namespace fs = boost::filesystem;

  fs::path rootPath(input);
  if(!fs::exists(rootPath))
  {
    cout << "intput path not exists" << endl;
    return false;
  }
  fs::recursive_directory_iterator endIter;
  for(fs::recursive_directory_iterator it(rootPath);it != endIter;++it)
  {
    if(!fs::is_regular_file(*it))
    {
      continue;
    }
    if(it->path().extension() != ".html")
    {
      continue;
    }
    output->push_back(it->path().string());

  }

  return true;
}
bool parseTitle(const string&input,string*title)
{
  size_t beg = input.find("<title>");
  if(beg == string::npos)
  {
    cout << "parse title failed,no beg" << endl;
    return false;
  }
  size_t end = input.find("</title>");
  if(end == string::npos)
  {
    cout << "parse title failed,no end" << endl;
    return false;
  }

  beg += string("<title>").size();
  *title = input.substr(beg,end - beg);

  return true;
}

bool parseUrl(const string&input,string*title)
{

}

bool parseContent(const string&input,string*title)
{

}

bool ParseFile(const string&input,DocInfo* docInfo) 
{
  string html;
  bool ret = common::Util::read(input,&html);
  if(!ret)
  {
    cout << "read file failed" << endl;
    return false;
  }
  
  //解析title
  ret = parseTitle(html,&docInfo->title);
  if(!ret)
  {
    cout << "parse title failed" << endl;
    return false;
  }

  //解析URL
  ret = parseUrl(input,&docInfo->url);
  if(!ret)
  {
    cout << "parse url failed" << endl;
    return false;
  }
  
  //解析content
  ret = parseContent(html,&docInfo->content);
  if(!ret)
  {
    cout << "parse content failed" << endl;
    return false;
  }

  return true;

}

int main()
{
  vector<string> fileList;
  // 将文件枚举读入一个临时文件里,为后续处理做准备
  if(!EnumFile(g_input_path,&fileList))
  {
    cout << "enum file failed" << endl;
    return 1;
  }
  // 依次解析每一个文件的内容
  ofstream outPutFile(g_output_path.c_str());
  if(!outPutFile.is_open())
  {
    cout << "open file failed" << endl;
    return 2;
  }

  for(const auto &f : fileList)
  {
    cout << "parse file: " << f << endl;
    DocInfo docInfo;
    if(!ParseFile(f,&docInfo))
    {
      cout << "parse file failed" << endl;
      continue;
    }
  }

  return 0;
}
