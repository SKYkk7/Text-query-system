#include "Query.h"
#include <algorithm>

QueryResult OrQuery::eval(const TextQuery& text) const
{
	// ͨ��Query��Աlhs��rhs���������
	// ����eval����ÿ����������QueryResult
	auto right = rhs.eval(text), left = lhs.eval(text);
	// ��������������кſ��������set��
	auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), left.end());
	// �����Ҳ�������������к�
	ret_lines->insert(right.begin(), right.end());

	return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult AndQuery::eval(const TextQuery& text) const
{
	// ͨ��query���������е�����ã��Ի���������Ĳ�ѯ���set
	QueryResult left = lhs.eval(text), right = rhs.eval(text);
	// ����left��right������set
	std::shared_ptr<std::set<line_no>>
		ret_lines = std::make_shared<std::set<line_no>>();

	std::set_intersection(left.begin(), left.end(), right.begin(), right.end(), std::inserter(*ret_lines, ret_lines->begin()));

	return QueryResult(rep(), ret_lines, left.get_file());
}

QueryResult NotQuery::eval(const TextQuery& text) const
{
	// ͨ��Query��������eval���������
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