#include "parser.hpp"
#include <string>
#include <iostream>

std::string json_str = R"({
    "a": ["bb", "c"] 
})";



int main(int argc, char *argv[]) {
    namespace x3 = boost::spirit::x3;
    using json::grammer::value;
    json::ast::value root;
    auto iter = json_str.begin();
    auto end = json_str.end();
    using boost::spirit::x3::ascii::space;
    bool r = x3::phrase_parse(iter, end, value, space, root);
    if (r && iter == end) {
        std::cout << "Success!!" << std::endl;
        json::ast::value_printer printer;
        boost::apply_visitor(printer(), root);
    } else {
        std::string context(iter, iter + 30 > end ? end : iter+30);
        std::cout << "Failed!  " << context << std::endl;
    }
   
} 


