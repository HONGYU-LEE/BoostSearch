#include<iostream>
#include<fstream>

#include<string>
#include<vector>
#include<unordered_map>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include"../common/util.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::unordered_map;
using std::ofstream;

struct DocInfo
{
    string title;
    string url;
    string context;
};

string g_input_path = "../data/input/";
string g_output_path = "../data/temp/raw_input";

bool enumFile(const string& input_path, vector<string>* file_list)
{
    //通过使用boost库中的filesysytem模块来遍历目录
    namespace fs = boost::filesystem;

    fs::path file_path(input_path);
    if(!fs::exists(file_path))
    {
        cout << "input path not exists." << endl;
        return false;
    }

    fs::recursive_directory_iterator end;//默认构造的迭代器为end
    for(fs::recursive_directory_iterator it(file_path); it != end; it++)
    {
        //过滤非常规文件
        if(!fs::is_regular_file(*it))
        {
            continue;
        }

        //过滤不是html的文件
        if(it->path().extension() != ".html")
        {
            continue;
        }

        file_list->push_back(it->path().string());
    }

    return true;
}

bool parserTitle(const string& html, string* title)
{
    return true;
}

bool parserContext(const string& html, string* context)
{
    return true;
}

bool parserURL(const string& html, string url)
{
    return true;
}

int main()
{
    vector<string> file_list;

    if(!enumFile(g_input_path, &file_list))
    {
        cout << "enumFile file." << endl;
        return -1;
    } 

    /*ofstream outputFile(g_output_path);
    if(!outputFile.is_open())
    {
        cerr << "open output file fail." << endl;
        return -1;
    }
    */
    for(const auto& file : file_list)
    {
        cout << "parse file : " << file << endl;
    }

    //outputFile.close();
    return 0;
}
