#include"searcher.hpp"

int main()
{
    searcher::Searcher searcher;
    if(!searcher.Init("../data/temp/raw_input"))
    {
        cout << "搜索模块构建失败！！！" << endl;
        return 1;
    }

    while (1) 
    {
        // std::flush用于刷新缓冲区
        std::cout << "searcher> " << std::flush;

        string query;
        std::cin >> query;
        if (!std::cin.good()) 
        {
            // 读到 EOF则终止
            std::cout << "goodbye" << std::endl;
            break;
        }

        string results;
        searcher.Search(query, &results);

        std::cout << results << std::endl;
    }
    return 0;
}