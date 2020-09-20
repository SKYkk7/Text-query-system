#include "TextQuery.h"
#include <sstream>
#include <algorithm>

TextQuery::TextQuery(std::ifstream& ifs) : input(new std::vector<std::string>)
{
    LineNo lineNo{ 0 };  // ��ʼ��

    // ���ı�ÿһ�н��в�ѯ������lineNo����¼���鵥�������к�
    for (std::string line; std::getline(ifs, line); ++lineNo)
    {
        input->push_back(line);
        std::istringstream line_stream(line);

        // ��ÿһ����ÿһ�����ʽ���ƥ�䣬
        for (std::string text, word; line_stream >> text; word.clear())
        {
        
            std::remove_copy_if(text.begin(), text.end(), std::back_inserter(word), ispunct);
           
            auto& nos = result[word];
            if (!nos) nos.reset(new std::set<LineNo>);
            nos->insert(lineNo);
        }
    }
}

// ��ѯ����
QueryResult TextQuery::query(const std::string& str) const
{
    
    static std::shared_ptr<std::set<LineNo>> nodata(new std::set<LineNo>);
    auto found = result.find(str);
    if (found == result.end())  
        return QueryResult(str, nodata, input);  // ���ҳɹ�
    else 
        return QueryResult(str, found->second, input);   // ����ʧ��
}

// ��ӡ����
std::ostream& print(std::ostream& out, const QueryResult& qr)  
{
    out << qr.word << " occurs " << qr.nos->size() << (qr.nos->size() > 1 ? " times" : " time") << std::endl;
    for (auto i : *qr.nos)
        out << "\t(line " << i + 1 << ") " << qr.input->at(i) << std::endl;
    return out;
}