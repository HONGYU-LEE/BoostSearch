#include"../common/util.hpp"

struct DocInfo
{
    string title;
    string url;
    string content;
};

string g_input_path = "../data/input/";
string g_output_path = "../data/temp/raw_input";

//枚举文件路径
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

        //过滤非html的文件
        if(it->path().extension() != ".html")
        {
            continue;
        }

        file_list->push_back(it->path().string());
    }

    return true;
}

//解析html中的标题
bool parserTitle(const string& html, string* title)
{
    size_t begin = html.find("<title>");
    if(begin == string::npos)
    {
        cout << "未找到标题！！！" << endl;
        return false;
    }

    size_t end = html.find("</title>");
    if(end == string::npos)
    {
        cout << "未找到标题！！！" << endl;
        return false;
    }

    begin += string("<title>").size();
    if(begin >= end)
    {
        cout << "标签位置不合法！！！" << endl;
        return false;
    }
    
    *title = html.substr(begin, end - begin);

    return true;
}

//解析html中的URL
bool parserURL(const string& file_path, string* url)
{
    string head = "https://www.boost.org/doc/libs/1_74_0/doc/html/";
    string tail = file_path.substr(g_input_path.size());

    *url = (head + tail);

    return true;
}

//解析html中的正文
bool parserContent(const string& html, string* content)
{
    bool is_content = true;

    //过滤掉标签中的内容
    for(auto ch : html)
    {
        if(is_content)
        {
            if(ch == '<')
            {
                is_content = false;
            }
            else
            {
                //将换行改为空格，使得输出为一个行文本文件，每一行对应着一个html文件
                if(ch == '\n')
                {
                    ch = ' ';
                }
                content->push_back(ch);
            }
        }
        else
        {
            if(ch == '>')
            {
                is_content = true;
            }
        }
    }

    return true;
}

//解析文件
bool parserFile(const string& file_path, DocInfo* doc_info)
{
    //读取html文件内容
    string html;
    if(!common::Util::Read(file_path, &html))
    {
        cout << "html文件读取失败！！！" << endl;
        return false;
    }

    //解析标题
    if(!parserTitle(html, &doc_info->title))
    {
        cout << "标题解析失败！！！" << endl;
        return false;
    }

    //解析URL
    if(!parserURL(file_path, &doc_info->url))
    {
        cout << "URL解析失败！！！" << endl;
        return false;
    }

    //解析正文
    if(!parserContent(html, &doc_info->content))
    {
        cout << "正文解析失败！！！" << endl;
        return false;
    }

    return true;
}

void Write(const DocInfo& doc_info, ofstream& ofstream)
{
    //使用不可见字符\1来划分内容，方便之后的读取
    ofstream << doc_info.title << "\1" << doc_info.url << "\1" << doc_info.content << endl;
}

int main()
{
    vector<string> file_list;

    //枚举出文件的所在路径
    if(!enumFile(g_input_path, &file_list))
    {
        cout << "枚举文件失败！！！" << endl;
        return -1;
    } 

    ofstream output_file(g_output_path.c_str());
    if(!output_file.is_open())
    {
        cout << "输出文件打开失败！！！" << endl;
        return -1;
    }
    
    //解析路径中的所有文件
    for(const auto& file : file_list)
    {
        cout << file << endl;

        DocInfo doc_info;
        if(!parserFile(file, &doc_info))
        {
            cout << "该文件解析失败:" << file << endl;
            continue;
        }
        //将解析出的信息写入输出文件中
        Write(doc_info, output_file);
    }   
    output_file.close();
    return 0;
}
