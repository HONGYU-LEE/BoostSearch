#include"index.hpp"

using namespace Indexes;

int main()
{
    Index index;

    //构建索引
    if(!index.Build("../data/temp/raw_input"))
    {
        cout << "索引构建失败！！！" << endl;
        return 1;
    }

    //在倒排索引中查找关键词
    auto* invert_list = index.getInvertedList("filesystem");
    if(!invert_list)
    {
        cout << "关键词未找到！！！" << endl;
        return 1;
    }
    
    //遍历倒排拉链中的Weight权重对象
    for(const auto& weight : *invert_list)
    {
        cout << "id : " << weight.doc_id << endl;

        //通过使用doc_id在正排索引中找到doc_info
        DocInfo* doc_info = index.getDocInfo(weight.doc_id);
        if(!doc_info)
        {
            cout << "文档未找到！！！" << endl;
            return 1;
        }

        cout << "title : " << doc_info->title << endl;
        cout << "url : " << doc_info->url << endl;
        cout << "content : " << doc_info->content << endl;
        cout << "////////////////////////////////////////////////////////////////" << endl;
    }
    
    return 0;
}