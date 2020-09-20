#ifndef QUERY_H
#define QUERY_H

class TextQuery;
class QueryResult;
#include <iostream>
#include <string>
#include <memory>
#include "TextQuery.h"


class Query_base
{
    friend class Query;
protected:
    using line_no = TextQuery::LineNo; 
    virtual ~Query_base() = default;

private:
    // 返回与此查询匹配的QueryResult
    virtual QueryResult eval(const TextQuery&) const = 0;

    // 查询的字符串表示形式
    virtual std::string rep() const = 0;
};

// Query_base的派生类，用于查找一个给定的单词
class WordQuery : public Query_base
{
    friend class Query;
    WordQuery(const std::string& s) :
        query_word(s)
    {
        std::cout << "WordQuery::WordQuery(" + s + ")\n";
    }


    QueryResult eval(const TextQuery& t) const override
    {
        return t.query(query_word);
    }
    std::string rep() const override
    {
        std::cout << "WodQuery::rep()\n";
        return query_word;
    }


    std::string query_word;
};

// 一个接口类，指向Query_base派生类的对象
class Query
{
    friend Query operator~(const Query&);
    friend Query operator|(const Query&, const Query&);
    friend Query operator&(const Query&, const Query&);

public:
    
    Query(const std::string& s) : q(new WordQuery(s))
    {
        std::cout << "Query::Query(const std::string& s) where s=" + s + "\n";
    }

    // 调用相应的基于查询的操作
    QueryResult eval(const TextQuery& t) const
    {
        return q->eval(t);
    }
    std::string rep() const
    {
        std::cout << "Query::rep() \n";
        return q->rep();
    }

private:
    // 仅适用友元的构造函数
    Query(std::shared_ptr<Query_base> query) :
        q(query)
    {
        std::cout << "Query::Query(std::shared_ptr<Query_base> query)\n";
    }
    std::shared_ptr<Query_base> q;
};

inline std::ostream&
operator << (std::ostream& os, const Query& query)
{
    // 通过指向Query_base的指针进行虚调用rep函数
    return os << query.rep();
}

// Query_base派生出来的另外一个抽象基类，表示有两个运算对象的查询
class BinaryQuery : public Query_base
{
protected:
    BinaryQuery(const Query& l, const Query& r, std::string s) :
        lhs(l), rhs(r), opSym(s)
    {
        std::cout << "BinaryQuery::BinaryQuery()  where s=" + s + "\n";
    }


    std::string rep() const override
    {
        std::cout << "BinaryQuery::rep()\n";
        return "(" + lhs.rep() + " "
            + opSym + " "
            + rhs.rep() + ")";
    }

    Query lhs, rhs;
    std::string opSym;
};

// BinaryQuery的派生类，返回它的两个运算对象分别出现的行的并集
class OrQuery :public BinaryQuery
{
    friend Query operator|(const Query&, const Query&);
    OrQuery(const Query& left, const Query& right) :
        BinaryQuery(left, right, "|")
    {
        std::cout << "OrQuery::OrQuery\n";
    }

    QueryResult eval(const TextQuery&)const override;
};

inline Query operator|(const Query& lhs, const Query& rhs)
{
    return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}


// BinaryQuery的派生类，返回它的两个运算对象分别出现的行的并集 
class AndQuery : public BinaryQuery
{
    friend Query operator&(const Query&, const Query&);
    AndQuery(const Query& left, const Query& right) :
        BinaryQuery(left, right, "&")
    {
        std::cout << "AndQuery::AndQuery()\n";
    }


    QueryResult eval(const TextQuery&) const override;
};

inline Query operator& (const Query& lhs, const Query& rhs)
{
    return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}


// Query_base的派生类，查询结果是Query运算对象没有出现的行的集合
class NotQuery : public Query_base
{
    friend Query operator~(const Query& operand);
    NotQuery(const Query& q) : query(q)
    {
        std::cout << "NotQuery::NotQuery()\n";
    }

    std::string rep() const override
    {
        std::cout << "NotQuery::rep()\n";
        return "~(" + query.rep() + ")";
    }

    QueryResult eval(const TextQuery&) const override;

    Query query;
};

inline Query operator~(const Query& operand)
{
    return std::shared_ptr<Query_base>(new NotQuery(operand));

}
#endif // QUERY_H
