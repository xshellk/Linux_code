#pragma once 

#include<string>
#include<vector>
#include<unordered_map>
#include<stdint.h>
#include"cppjieba/Jieba.hpp"

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
  //倒排索引所需要的id以及该分词结果中,该id对应的文件中,该分词所占有的权重
  //权重可以通过 标题出现次数*10 + 正文次数 来计算
  //
  int64_t docId;
  string word;
  int weight;
};
    
typedef vector<Weight> InvertedList;

class Index{
private:
  vector<DocInfo> forwardIndex;
  unordered_map<string,InvertedList> invertedIndex;

public:
  Index();
  //根据文档id获得索引内容
  const DocInfo* getDocInfo(int64_t docId);
  //根据词,获取倒排拉链
  const InvertedList*getInverted(const string&key);
  //构建索引
  //inputPath就是指向raw_input文件
  bool build(const string& inputPath);

  void cutWord(const string& word, vector<string> *tokens);


private:
  //juild方法内部所需要使用的方法
  DocInfo* buildForward(const string&line);
  void buildInverted(const DocInfo& docInfo);
  cppjieba::Jieba jieba;

};
//////////////////////////////////////////////////////
class Searcher{
private:
  Index* index;

public:
  Searcher():index(new Index())
  {}
  bool init(const std::string inputPath); 
  bool search(const std::string query,std::string * result);
  string generateDesc(const string&content,const string &word);

};


}

