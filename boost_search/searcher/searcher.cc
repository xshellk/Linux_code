#include<iostream>
#include<fstream>
#include<algorithm>
#include"../common/Util.hpp"
#include"searcher.h"
#include<jsoncpp/json/json.h>


using namespace std;


namespace searcher{

const char* const DICT_PATH = "../jieba_dict/jieba.dict.utf8";
const char* const HMM_PATH = "../jieba_dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../jieba_dict/user.dict.utf8";
const char* const IDF_PATH = "../jieba_dict/idf.utf8";
const char* const STOP_WORD_PATH = "../jieba_dict/stop_words.utf8";

Index::Index() : jieba(DICT_PATH,
    HMM_PATH,
    USER_DICT_PATH,
    IDF_PATH,
    STOP_WORD_PATH
    ){}

const DocInfo* Index::getDocInfo(int64_t docId)
{
  //直接通过文档的标号进行正排索引的查找
  if(docId > (int64_t)forwardIndex.size())
  {
    return nullptr;
  }
  return &forwardIndex[docId];
}
const InvertedList* Index::getInverted(const string&key)
{
  //查找哈希表来寻找分词对应的vector<InvertedList>结构
  //该结构包括了所有包含个该分词的文档的倒排索引
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
  //按行读取文件的内容,每一行就是之前的单个HTML文件所提取出的内容
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
    if(docInfo->docId % 100 == 0)
    {
      cout << "Index build docId:" << docInfo->docId << endl;
    }
    
  }
  file.close();
  cout << "build finish" << endl;

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
  docInfo.docId = forwardIndex.size();
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
  struct WordCnt{
    int titleCnt;
    int contentCnt;
    WordCnt():titleCnt(0),contentCnt(0)
    {}
  };

  unordered_map<string,WordCnt> wordCntMap;

  //从docInfo拿出title和正文,分词
  
  //先统计标题
  vector<string> titleToken;
  cutWord(docInfo.title,&titleToken);
  for(string word : titleToken)
  {
    boost::to_lower(word);
    ++wordCntMap[word].titleCnt;
  }

  //统计正文中的次数
  vector<string> contentToken;
  cutWord(docInfo.content,&contentToken);
  for(string word : contentToken)
  {
    boost::to_lower(word);
    ++wordCntMap[word].contentCnt;
  }
  //将分词结果放入一张map里
  //根据map里存储的分词结果,计算对应的分词权重, 将其放入倒排拉链中去
  for(const auto &wordPair : wordCntMap)
  {
    Weight weight;
    weight.docId = docInfo.docId;
    weight.weight = wordPair.second.titleCnt * 10 + wordPair.second.contentCnt;
    weight.word = wordPair.first;

    InvertedList& invertedList = invertedIndex[wordPair.first];
    invertedList.push_back(weight);
    
  }
}


void Index::cutWord(const string &word,vector<string>* tokens)
{
  jieba.CutForSearch(word,*tokens);
}

bool Searcher::init(const string inputPath)
{
  return index->build(inputPath);
}

bool Searcher::search(const string query,string *output)
{
  vector<string> tokens;
  index->cutWord(query,&tokens);

  vector<Weight> allTokenResult;

  for(string word : tokens)
  {
    boost::to_lower(word);
    const auto * invertedList = index->getInverted(word);
    if(invertedList == nullptr)
      continue;

    allTokenResult.insert(allTokenResult.end(),invertedList->begin(),invertedList->end());
  }
  std::sort(allTokenResult.begin(),allTokenResult.end(),
      [](const Weight& w1,const Weight& w2){
      return w1.weight > w2.weight;
      });

  Json::Value results;
  for(const auto &weight : allTokenResult)
  {
    const auto *docInfo = index->getDocInfo(weight.docId);
    Json::Value result;
    result["title"] = docInfo->title;
    result["url"] = docInfo->url;
    result["desc"] = generateDesc(docInfo->content,weight.word);
    results.append(result);
  }
  Json::FastWriter writer;
  *output = writer.write(results);
  return true;
}

string Searcher::generateDesc(const string &content,const string &word)
{
  int64_t pos = content.find(word);
  if(pos == (int64_t)string::npos)
  {
    return "";
  }
  int64_t beg = pos < 60 ? 0 : pos;
  if(beg + 160 > (int64_t)content.size())
    return content.substr(beg);
  else 
  {
    string desc = content.substr(beg,160);
    desc[desc.size() - 1] = '.';
    desc[desc.size() - 2] = '.';
    desc[desc.size() - 3] = '.';
    return desc;
  }
}

}
