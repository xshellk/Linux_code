#include<iostream>
#include"searcher.h"

int main()
{
  using namespace std;
  searcher::Searcher searcher;
  bool ret = searcher.init("../data/tmp/raw_input");
  if(!ret)
  {
    cout << " init failed" << endl;
    return 1;
  }
  while(true)
  {
    string query;
    cout << "searcher> " << flush;
    cin >> query;
    if(!cin.good())
    {
      cout << "goodBye!" << endl;
    }
    string output;
    searcher.search(query,&output);
    cout << "output : " << endl;
    cout << output << endl;

  }
  
   


  return 0;
}
