#include<iostream>
#include<fstream>

#include"../common/Util.hpp"
#include"searcher.h"

using namespace std;


namespace searcher{

const DocInfo* Index::getDocInfo(int64_t docId)
{
  if(docId > (int64_t)forwardIndex.size())
  {
    return nullptr;
  }
  return &forwardIndex[docId];
}
const InvertedList* Index::getInverted(const string&key)
{
  auto it = invertedIndex.find(key);
  if(it != invertedIndex.end())
  {
    return &it->second;
  }
  return nullptr;
}

bool Index::build(const string&inputPath)
{
  cout << "build start" << endl;
  ifstream file(inputPath.c_str());

  if(!file.is_open())
  {
    cout << "open for build failed, inputPaht is " << inputPath << endl;
    return false;
  }
  string line;
  while(getline(file,line))
  {
    //通过line,来构建正排索引
    DocInfo* docInfo = buildForward(line);
    if(nullptr == docInfo)
    {
      continue;
    }
    //构建倒排索引
    buildInverted(*docInfo);
    
  }

  return true;
}
DocInfo* Index::buildForward(const string&line)
{
  vector<string> token;
  //对line进行截取,并放进token中
  common::Util::split(line,"\3",&token);
  if(token.size() != 3)
  {
    return nullptr;
  }
  DocInfo docInfo;
  docInfo.title = token[0];
  docInfo.url = token[1];
  docInfo.content = token[2];
  //将token所存储的数据放入构建好的docInfo中
  //使用右值引用提高效率
  forwardIndex.push_back(std::move(docInfo));
  //返回刚刚填充好的正排索引的地址
  return &forwardIndex.back();
}
void Index::buildInverted(const DocInfo& docInfo)
{

}

}
