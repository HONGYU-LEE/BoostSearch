#pragma once 

#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <unordered_map>

#include <memory>
#include <algorithm>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <json/json.h>

#include "../cppjieba/Jieba.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::ifstream;
using std::ofstream;
using std::getline;

namespace common
{
    class Util
    {
        public:
        //将html文档中的内容提取出来
        static bool Read(const string& input_path, string* output)
        {
            std::ifstream file(input_path.c_str());
            if(!file.is_open())
            {
                cout << "文件打开失败！！！" << endl;
                return false;
            } 

            string line;
            while(getline(file, line))
            {
                *output += (line + '\n');
            }
            file.close();

            return true;
        }

        /*
            封装boost的split
            delimiter:分隔符集合
            token_compress:是否开启切分压缩
        */
        static void Split(const string &input, const string &delimiter, vector<string> *output)
        {
            boost::split(*output, input, boost::is_any_of(delimiter), boost::token_compress_off);
        }
    };
};  //namespace common