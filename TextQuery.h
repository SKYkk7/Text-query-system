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

// ���������ı�
class TextQuery {
public:
    using LineNo = std::vector<std::string>::size_type;
    TextQuery(std::ifstream&);   // ͨ��һ��������������
    QueryResult query(const std::string&) const;  // ����һ���ַ������в�ѯ������һ��QueryResult
private:
    std::shared_ptr<std::vector<std::string>> input;   // ����������ı�
    std::map<std::string, std::shared_ptr<std::set<LineNo>>> result;   // ��ѯ���
};

// �����ѯ�������
class QueryResult {
public:
    friend std::ostream& print(std::ostream&, const QueryResult&);    // ͨ��һ�����������ӡ��ѯ���
public:
    QueryResult(const std::string& s, std::shared_ptr<std::set<TextQuery::LineNo>> set, std::shared_ptr<std::vector<std::string>> v) : word(s), nos(set), input(v) { }

    std::set<TextQuery::LineNo>::iterator begin() { return nos->begin(); }
    std::set<TextQuery::LineNo>::iterator end() { return nos->end(); }
    std::shared_ptr<std::vector<std::string>> get_file() { return input; }

private:
    std::string word;  // ��ѯ�ĵ���
    std::shared_ptr<std::set<TextQuery::LineNo>> nos;   // ͨ��set���浥�������к�
    std::shared_ptr<std::vector<std::string>> input;    // ����������ı�
};

std::ostream& print(std::ostream&, const QueryResult&);   

#endif // !TEXTQUERY_H

