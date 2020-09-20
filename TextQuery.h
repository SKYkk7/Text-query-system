#ifndef TEXTQUERY_H
#define TEXTQUERY_H

class QueryResult;

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <map>
#include <set>

class QueryResult;

// 保存输入文本
class TextQuery {
public:
    using LineNo = std::vector<std::string>::size_type;
    TextQuery(std::ifstream&);   // 通过一个输入流来构造
    QueryResult query(const std::string&) const;  // 接受一个字符串进行查询，返回一个QueryResult
private:
    std::shared_ptr<std::vector<std::string>> input;   // 保存输入的文本
    std::map<std::string, std::shared_ptr<std::set<LineNo>>> result;   // 查询结果
};

// 保存查询结果的类
class QueryResult {
public:
    friend std::ostream& print(std::ostream&, const QueryResult&);    // 通过一个输出流来打印查询结果
public:
    QueryResult(const std::string& s, std::shared_ptr<std::set<TextQuery::LineNo>> set, std::shared_ptr<std::vector<std::string>> v) : word(s), nos(set), input(v) { }

    std::set<TextQuery::LineNo>::iterator begin() { return nos->begin(); }
    std::set<TextQuery::LineNo>::iterator end() { return nos->end(); }
    std::shared_ptr<std::vector<std::string>> get_file() { return input; }

private:
    std::string word;  // 查询的单词
    std::shared_ptr<std::set<TextQuery::LineNo>> nos;   // 通过set保存单词所在行号
    std::shared_ptr<std::vector<std::string>> input;    // 保存输入的文本
};

std::ostream& print(std::ostream&, const QueryResult&);   

#endif // !TEXTQUERY_H

