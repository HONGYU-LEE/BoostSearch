#pragma once

#include"../common/util.hpp"

namespace indexes
{
    //正排索引：根据文档ID来查找文档正文
    //倒排索引：根据文档正文(词)来找到文档ID

    //存储html文档的信息
    struct DocInfo
    {
        int64_t doc_id;
        string title;
        string url;
        string content;
    };

    //权重信息
    struct Weight
    {
        int64_t doc_id;
        int weight; //权重
        string word;
    };
    //倒排拉链
    typedef vector<Weight> InvertedList;

    class Index
    {
        private:
        vector<DocInfo> forward_index;  //正排索引
        unordered_map<string, InvertedList> invert_index;   //倒排索引
        cppjieba::Jieba jieba;  //结巴分词

        DocInfo* buildForwardIndex(string line);
        void buildInvertIndex(const DocInfo& doc_info);
        
        public:
        Index();
        DocInfo* getDocInfo(int64_t doc_id);
        InvertedList* getInvertedList(const string& key);
        bool Build(const string& input_path);
        void CutWord(const string& input, vector<string>* output);
    };

    //结巴分词词典位置
    const char* const DICT_PATH = "../cppjieba/dict/jieba.dict.utf8";
    const char* const HMM_PATH = "../cppjieba/dict/hmm_model.utf8";
    const char* const USER_DICT_PATH = "../cppjieba/dict/user.dict.utf8";
    const char* const IDF_PATH = "../cppjieba/dict/idf.utf8";
    const char* const STOP_WORD_PATH = "../cppjieba/dict/stop_words.utf8";

    Index::Index() 
        : jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH) 
    {}

    //分词
    void Index::CutWord(const string& input, vector<string>* output)
    {
        jieba.CutForSearch(input, *output);
    }

    //构建正排索引
    DocInfo* Index::buildForwardIndex(string line)
    {
        vector<string> tokens;

        common::Util::Split(line, "\1", &tokens);//根据\1划分内容
        if(tokens.size() != 3)
        {
            //如果划分出来的属性不等于三个，说明数据有问题
            cout << "数据划分错误！！！" << endl;
            return nullptr;
        }

        DocInfo doc_info;
        doc_info.doc_id = forward_index.size();
        doc_info.title = tokens[0];
        doc_info.url = tokens[1];
        doc_info.content = tokens[2];

        forward_index.push_back(std::move(doc_info));//使用移动语义来利用将亡值

        return &forward_index.back();
    }

    //构建倒排索引
    void Index::buildInvertIndex(const DocInfo& doc_info)
    {
        //词频统计结构
        struct WordCount
        {
            int title_count; //标题出现频率
            int content_count;   //正文出现频率

            WordCount() 
                : title_count(0)
                , content_count(0)
            {}
        };

        unordered_map<string, WordCount> word_count_map; //利用哈希表进行统计

        //对标题进行分词
        vector<string> title_tokens;
        CutWord(doc_info.title, &title_tokens);

        //统计词频，这里统一将单词转为小写
        for(string word : title_tokens)
        {
            boost::to_lower(word);
            ++word_count_map[word].title_count;
        }

        //对正文进行分词
        vector<string> content_tokens;
        CutWord(doc_info.content, &content_tokens);

        for(string word : content_tokens)
        {
            boost::to_lower(word);
            ++word_count_map[word].content_count;
        }

        //根据词频统计表计算出权重，整合出Weight对象，并更新到倒排索引中
        for(const auto& word_pair : word_count_map)
        {
            Weight weight;
            weight.doc_id = doc_info.doc_id;
            //这里采用线性权重计算, 权重公式如下
            //权重 = 标题出现次数 * 10 + 正文出现次数
            weight.weight = word_pair.second.title_count * 10 + word_pair.second.content_count;
            weight.word = word_pair.first;

            //找到对应的倒排拉链，将权重尾插进去
            InvertedList& invert_list  = invert_index[word_pair.first];
            invert_list.push_back(std::move(weight));
        }
    }

    //构建索引
    bool Index::Build(const string& input_path)
    {
        cout << "索引构建开始！！！" << endl;
        
        ifstream file(input_path.c_str());
        if(!file.is_open())
        {
            cout << "raw_input 文件打开失败！！！" << endl;
            return false;
        }

        string line;
        while(getline(file, line))
        {
            DocInfo* doc_info = Index::buildForwardIndex(line);
            if(doc_info == nullptr)
            {
                //当前文档构建正排索引失败，跳过本轮
                cout << "正排索引构建失败！！！" << endl;
                continue;
            }

            //根据doc_info来构建倒排索引
            Index::buildInvertIndex(*doc_info);

            /*
                抽空补充一个进度条
            */
        } 

        cout << "索引构建结束！！！" << endl;
        file.close();
        return true;
    } 

    //查找正排索引
    DocInfo* Index::getDocInfo(int64_t doc_id) 
    {
        if(doc_id < 0 || doc_id >= forward_index.size())
        {
            return nullptr;
        }

        return &forward_index[doc_id];
    }

    //查找倒排索引
    InvertedList* Index::getInvertedList(const string& key) 
    {
        auto it = invert_index.find(key);
        if(it == invert_index.end())
        {
            return nullptr;
        }
        
        return &it->second;
    }
};  //namespace indexer


