#include "cpp-httplib/httplib.h"
#include "../searcher/searcher.hpp"
int main()
{
    using namespace httplib;

    searcher::Searcher searcher;
    //初始化搜索引擎
    if(!searcher.Init("../data/temp/raw_input"))
    {
        cout << "searcher初始化失败！！！" << endl;
        return 1;
    }

    Server server;  //创建服务器对象
    //获取请求信息
    server.Get("/searcher", [&searcher](const Request& req, Response& resp){
        if(!req.has_param("query"))
        {
            resp.set_content("请求的参数错误！！！", "text/plain; charset=utf-8");
            return;
        }

        //获取查询词进行查询
        string query = req.get_param_value("query");
        string results;
        searcher.Search(query, &results);
        
        //查询后响应查询的结果
        resp.set_content(results, "application/json; charset=utf-8");
    });

    server.set_base_dir("./html");   //设置html静态资源路径
    server.listen("0.0.0.0", 9188);  //服务器开始监听
    
    return 0;
}