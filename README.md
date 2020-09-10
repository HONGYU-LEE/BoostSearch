# BoostSearch / 搜索引擎
## 开发语言
C/C++，HTML，CSS，JavaScript
## 开发环境
CentOS7、vim、g++、gdb、git、Makefile
## 依赖库
jsoncpp, cppjieba, http-lib, boost
## 项目简介
BoostSearch是一个基于boost文档的站内搜索引擎，当用户在页面上输入查询词后，就会快速的查询出相关的boost在线文档，弥补了boost在线文档中没有搜索的缺陷。
## 项目特点
- 对离线版本的html文档进行解析，将解析结果整理为一个行本文文件。
- 读取处理好的行文本文件进行分词、权重计算等操作，在内存中构造出正排索引和倒排索引。
- 对查询词进行分词、触发，依据相关度对查询结果进行排序，并以Json格式进行包装后序列化为字符串返回。
- 通过HTTP服务器搭载搜索页面， 为外部提供服务。
## 演示链接
http://118.178.139.161:8080/index.html

# 介绍
![image](https://github.com/HONGYU-LEE/BoostSearch/blob/master/doc/image/1.png)
![image](https://github.com/HONGYU-LEE/BoostSearch/blob/master/doc/image/2.png)
![image](https://github.com/HONGYU-LEE/BoostSearch/blob/master/doc/image/3.png)
