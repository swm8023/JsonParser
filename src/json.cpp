#include "parser.hpp"
#include <string>
#include <iostream>
#include <boost/spirit/home/x3.hpp>

std::string json_str = R"({
    "a": "bb"
})"; 

int main(int argc, char *argv[]) {
    namespace x3 = boost::spirit::x3;
    using json::grammer::value_root;
    json::ast::value_root root;
    std::string::const_iterator iter = json_str.begin();
    std::string::const_iterator end = json_str.end();
    bool r = x3::phrase_parse(iter, end, value_root, x3::ascii::space, root);
    // if (r && iter == end) {
    //     std::cout << "Success!!" << std::endl;
    // } else {
    //     std::string context(iter, iter + 30 > end ? end : iter+30);
    //     std::cout << "Failed!  " << context << std::endl;
    // }

} 