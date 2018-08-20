#include "parser_spirit_x3.hpp"
#include <iostream>

std::string json_str = R"(
{
	"title": "example glossary",
	"GlossList": {
		"ID": 123,
		"SortAs": ["aa", "bb", true, 12.5, {
			"t0": true,
			"t1": "666"
		}],
		"Abbrev": "ISO 8879:1986",
		"GlossDef": {
			"para": "A meta-markup language, used to create markup languages such as DocBook.",
			"GlossSeeAlso": ["GML", "XML"]
		},
		"GlossSee": "markup"
	}
}
)";

namespace json {namespace parser {namespace ast {
struct Printer {
    Printer(int tab_num) : tab(tab_num) {}

    void operator()(Object const& v) const {
        std::cout << '{' << std::endl;
        for (auto const& item : v) {
            std::cout << tabstr(tab + 1) << '"' << item.first << "\" : ";
            boost::apply_visitor(Printer(tab + 1), item.second);
        }
        std::cout << tabstr(tab) << '}' << std::endl;
    }

    void operator()(Array const& v) const {
        std::cout << '[' << std::endl;
        for (auto const& item : v) {
            std::cout << tabstr(tab + 1);
            boost::apply_visitor(Printer(tab + 1), item);
        }
        std::cout << tabstr(tab) << ']' << std::endl;
    }

    void operator()(Null const& v) const {
        std::cout << "null" << std::endl;
    }

    void operator()(std::string const& v) const {
        std::cout << '"' << v << '"' << std::endl;
    }

    void operator()(bool const& v) const {
        std::cout << (v ? "true" : "false") << std::endl;
    }

    template<typename T>
    void operator()(T const& v) const {
        std::cout << v << std::endl;
    }

    std::string tabstr(int i, int tab_sz = 4) const {
        return std::string(i * tab_sz, ' ');
    }
    int tab = 0;
    
};
}}}


int main(int argc, char *argv[]) {
    namespace x3 = boost::spirit::x3;
    json::parser::ast::Value root;
    auto iter = json_str.begin();
    auto end = json_str.end();
    bool r = x3::phrase_parse(iter, end, json::parser::grammer::value, x3::ascii::space, root);
    if (r && iter == end) {
        std::cout << "Parse Success!!!" << std::endl;
        boost::apply_visitor(json::parser::ast::Printer(0), root);
    } else {
        std::cout << "Failed!!!" << std::endl;
    }

    
    return 0;
}