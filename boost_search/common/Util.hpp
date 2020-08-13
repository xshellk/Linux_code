#pragma once 

#include<string>
#include<fstream>


namespace common{
class Util{
public:
  static bool read(const std::string &input,std::string*output)
  {
    std::ifstream file(input.c_str(),std::ios::binary);
    if(!file.is_open())
    {
      return false;
    }
    //调整文件的光标位置,先将光标移动至最后位置,获取长度
    //再将光标移回至起始位置开始后续操作
    file.seekg(0,file.end);
    int length = file.tellg();
    file.seekg(0,file.beg);

    output->resize(length);
    file.read(const_cast<char*>(output->data()),length);
    file.close();
    return true;

  }

};

}
