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

bool parseUrl(const string&input,string*url)
{
  // 1.从input截取url的后缀
  string tail = input.substr(g_input_path.size());
  // 2.加上在线文档的路径前缀
  string head = "https://www.boost.org/doc/libs/1_53_0/doc";
  // 3.将构造好的放入title中
  *url = head + tail;
  return true;
}

bool parseContent(const string&html,string*content)
{
  bool isContent = true;
  string tag;
  //便利每一个html中的字符
  for(auto c : html)
  {
    //判断是不是正文
    if(isContent)
    {
      //判断是不是标签开始了
      if(c == '<')
        isContent = false;
      else 
      {
        //判断是不是换行符
        if(c == '\n')
          c = ' ';

        //将字符输出倒正文中
        content->push_back(c);
      }
    }
    else 
    {
      //如果标签结束了,就将siContent置为true
      //这里需要去除js和css这两个标签里的内容,因为这些内容是一些没有必要的内容
      if(c == '>')
      {
        isContent = true;
      }
    }
  }
  return true;
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
void writeOutput(const DocInfo&docInfo,ofstream &output)
{
  output << docInfo.title << "\3" << docInfo.url << "\3" << docInfo.content << endl;
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
    //cout << "parse file: " << f << endl;
    DocInfo docInfo;
    if(!ParseFile(f,&docInfo))
    {
      cout << "parse file failed" << endl;
      continue;
    }
    writeOutput(docInfo,outPutFile);
  }
  cout << "parse done" << endl;
  return 0;
}
