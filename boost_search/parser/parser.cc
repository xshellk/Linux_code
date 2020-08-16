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
  //获取命名空间的方法可以通过赋值操作来完成
  namespace fs = boost::filesystem;

  fs::path rootPath(input);
  if(!fs::exists(rootPath))
  {
    cout << "intput path not exists" << endl;
    return false;
  }
  //通过boost中的特殊迭代器访问该目录下的所有文件
  //如果是目录就跳过
  //如果不是以 ".html" 作为结尾的文件都是不要的
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
    //获得的路径信息放入output中
    output->push_back(it->path().string());
  }

  return true;
}
bool parseTitle(const string&input,string*title)
{
  //以title为分割,截取其中的内容
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
  //从title标签开始的位置向后跳title标签的长度
  //截取title的正文
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
  //读取input的文件到html中,input是一个路径信息
  //读取整个路径指向的文件的所有信息放入html中
  bool ret = common::Util::read(input,&html);
  if(!ret)
  {
    cout << "read file failed" << endl;
    return false;
  }
  
  //解析title
  //查找html文件的标签信息来得到title信息
  ret = parseTitle(html,&docInfo->title);
  if(!ret)
  {
    cout << "parse title failed" << endl;
    return false;
  }

  //解析URL
  //根据input路径信息,利用其构造一个在线路径信息
  ret = parseUrl(input,&docInfo->url);
  if(!ret)
  {
    cout << "parse url failed" << endl;
    return false;
  }
  
  //解析content
  //从html中读取所有的非标签正文信息
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
  //"\3" 是^C 标识符,选取一个不可见字符作为分割
  //"\n" 作为每一个html文件全部索引的分割
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

  //将得到的结果放入临时文件里
  for(const auto &f : fileList)
  {
    //cout << "parse file: " << f << endl;
    DocInfo docInfo;
    if(!ParseFile(f,&docInfo))
    {
      cout << "parse file failed" << endl;
      continue;
    }
    //写入文件
    writeOutput(docInfo,outPutFile);
  }
  cout << "parse done" << endl;
  return 0;
}
