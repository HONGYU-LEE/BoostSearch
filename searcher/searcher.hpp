#include"../common/util.hpp"
#include"../index/index.hpp"

namespace searcher
{
    class Searcher
    {
        private:
        indexes::Index* index;  //索引指针
        string generateDesc(const string& content, const string& word);

        public:
        Searcher() : 
            index(new indexes::Index()) //伴随整个程序运作周期，不需要delete，程序结束后自动销毁。
        {}
        bool Init(const string& input_path);
        bool Search(const string& query, string* results);

    };

    //初始化构建索引
    bool Searcher::Init(const string& input_path)
    {
        if(!index->Build(input_path))
        {
            cout << "搜索模块初始化失败！！！" << endl;
            return false;
        }
        
        return true;
    }

    //搜索
    bool Searcher::Search(const string& query, string* output)
    {
        //1.分词：将查询字符串进行分词
        vector<string> tokens;
        index->CutWord(query, &tokens);

        //2.触发：根据分词的结果在倒排索引中查找到相关的文档编号，再通过正排索引找到对应的文档
        vector<indexes::Weight> all_tokens;
        for(string word : tokens)
        {
            boost::to_lower(word);  //统一转为小写

            auto* invert_list = index->getInvertedList(word);
            if(invert_list == nullptr)
            {
                cout << "倒排索引中不存在当前词！！！" << endl;
                continue;
            }

            all_tokens.insert(all_tokens.end(), invert_list->begin(), invert_list->end());  //将搜索到的倒排拉链加入结果集中
        }

        //3.排序：根据权重信息来对Weight对象进行排序，将权重较高的高相关度文档放在前面
        std::sort(all_tokens.begin(), all_tokens.end(), [](const indexes::Weight& w1, const indexes::Weight& w2)->bool{
            return w1.weight > w2.weight;
        });

        //4.包装
        Json::Value results;    //以cppJson包装的结果集
        for(const auto& weight : all_tokens)
        {
            const auto* doc_info = index->getDocInfo(weight.doc_id);
            if(doc_info == nullptr)
            {
                cout << "该文档ID:" << weight.doc_id << "不存在" << endl;
                continue;
            }

            Json::Value result;
            result["title"] = doc_info->title;
            result["url"] = doc_info->url;
            result["desc"] = generateDesc(doc_info->content, weight.word);

            results.append(result);
        }

        //将得到的Json对象序列化为字符串，存入output中
        Json::StreamWriterBuilder builder;
        //std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        *output = Json::writeString(builder, results);
        //writer->write(results, *output);

        return true;
    }

    //从正文中提取描述信息
    string Searcher::generateDesc(const string& content, const string& word)
    {
        /*
        ·   （确保关键词尽量在前，描述更加准确。并且保证描述信息简短，如果后面还有数据则用省略号表示）
            描述信息构建规则
            返回内容 = 150字节 = 关键词往前50字节 + 关键词(包括关键词)往后100个字节
            1.正常情况下，选择关键词位置的前50个字节的位置作为起始位置，返回起始位置往后的150个字节。
            2.当前面不足50个时，就从起点位置往后直接返回150个字节。当末尾不足时，直接返回到结束位置。如果后面还有数据用省略号表示。
            3.如果正文中未找到关键词，则说明关键词在标题中，直接从起始位置开始返回150字节
        */

        size_t word_pos = content.find(word);
        size_t begin_pos = 0;

        if(word_pos == string::npos)
        {
            //此时说明关键词位于标题，直接返回160个字节的数据，如果长度不够则直接返回正文
            if(content.size() < 150)
            {
                return content;
            }
            
            string desc = content.substr(begin_pos, 147);
            desc += "...";

            return desc;
        }

        begin_pos = (word_pos > 50) ? word_pos - 50 : 0;
        //如果后续不够则直接返回到结尾
        if(begin_pos + 150 >= content.size())
        {
            return content.substr(begin_pos);
        }

        string desc = content.substr(begin_pos, 147);
        desc += "...";

        return desc;
    }
};  //namespace searcher
