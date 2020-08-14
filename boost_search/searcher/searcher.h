#pragma once 

#include<string>
#include<vector>
#include<unordered_map>
#include<stdint.h>

using std::string;
using std::vector;
using std::unordered_map; 

namespace searcher{
  //正排索引的结构体, 正排就是利用docId构建数组
struct DocInfo{
  int64_t docId;
  string title;
  string url;
  string content;
};

  //构建倒排索引的value
  //倒排索引也是键值对结构,key是分词结果,value就是Weight的对象,通过找到的对象,拿到docId
struct Weight{
  int64_t docId;
  int weight;
};
    
typedef vector<Weight> InvertedList;
class Index{
private:
  vector<DocInfo> forwardIndex;
  unordered_map<string,InvertedList> invertedIndex;

public:
  //根据文档id获得索引内容
  const DocInfo* getDocInfo(int64_t docId);
  //根据词,获取倒排拉链
  const InvertedList* getInverted(const string&key);
  //构建索引
  //inputPath就是指向raw_input文件
  bool build(const string& inputPath);

private:
  DocInfo* buildForward(const string&line);
  void buildInverted(const DocInfo& docInfo);

};



}

