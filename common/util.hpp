#pragma once 

#include<iostream>
#include<fstream>

#include<string>
#include<vector>
#include<unordered_map>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/constants.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::getline;

namespace common
{
    class Util
    {
        public:
        static bool Read(const string& input_path, string* output)
        {
            std::ifstream file(input_path.c_str());
            if(!file.is_open())
            {
                cout << "文件打开失败" << endl;
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
    };
};