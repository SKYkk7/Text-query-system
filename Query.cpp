#include "Query.h"
#include <algorithm>

QueryResult OrQuery::eval(const TextQuery& text) const
{
	// 通过Query成员lhs和rhs进行虚调用
	// 调用eval返回每个运算对象的QueryResult
	auto right = rhs.eval(text), left = lhs.eval(text);
	// 将左侧运算对象的行号拷贝到结果set中
	auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), left.end());
	// 插入右侧运算对象所在行号
	ret_lines->insert(right.begin(), right.end());

	return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult AndQuery::eval(const TextQuery& text) const
{
	// 通过query运算对象进行的虚调用，以获得运算对象的查询结果set
	QueryResult left = lhs.eval(text), right = rhs.eval(text);
	// 保存left和right交集的set
	std::shared_ptr<std::set<line_no>>
		ret_lines = std::make_shared<std::set<line_no>>();

	std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), std::inserter(*ret_lines, ret_lines->begin()));

	return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult NotQuery::eval(const TextQuery& text) const
{
	// 通过Query运算对象对eval进行虚调用
	auto result = query.eval(text);

	std::shared_ptr<std::set<line_no>>
		ret_lines = std::make_shared<std::set<line_no>>();

	std::set<TextQuery::LineNo>::iterator
		begin = result.begin(),
		end = result.end();

	std::vector<std::string>::size_type sz = result.get_file()->size();

	for (std::size_t n = 0; n != sz; ++n)
	{
		if (begin == end || *begin != n)
			ret_lines->insert(n);
		else if (begin != end)
			++begin;
	}

	return QueryResult(rep(), ret_lines, result.get_file());
}