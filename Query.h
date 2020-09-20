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
    // ������˲�ѯƥ���QueryResult
    virtual QueryResult eval(const TextQuery&) const = 0;

    // ��ѯ���ַ�����ʾ��ʽ
    virtual std::string rep() const = 0;
};

// Query_base�������࣬���ڲ���һ�������ĵ���
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

// һ���ӿ��ָ࣬��Query_base������Ķ���
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

    // ������Ӧ�Ļ��ڲ�ѯ�Ĳ���
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
    // ��������Ԫ�Ĺ��캯��
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
    // ͨ��ָ��Query_base��ָ����������rep����
    return os << query.rep();
}

// Query_base��������������һ��������࣬��ʾ�������������Ĳ�ѯ
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

// BinaryQuery�������࣬�������������������ֱ���ֵ��еĲ���
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


// BinaryQuery�������࣬�������������������ֱ���ֵ��еĲ��� 
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


// Query_base�������࣬��ѯ�����Query�������û�г��ֵ��еļ���
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
