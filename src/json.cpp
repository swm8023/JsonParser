#include "parser.hpp"
#include <string>
#include <iostream>

std::string json_str = R"(
{
"firstName": "John",
"lastName": "Smith",
"isAlive": true,
"age": 25,
"address": {
"streetAddress": "21 2nd Street",
"city": "New York",
"state": "NY",
"postalCode": "10021-3100"
},
"phoneNumbers": [
{
"type": "home",
"number": "212 555-1234"
},
{
"type": "office",
"number": "646 555-4567"
}
],
"children": [],
"spouse": null
}
)";



int main(int argc, char *argv[]) {
    namespace x3 = boost::spirit::x3;
    using json::grammer::value;
    json::ast::Value root;
    auto iter = json_str.begin();
    auto end = json_str.end();
    using boost::spirit::x3::ascii::space;
    bool r = x3::phrase_parse(iter, end, value, space, root);
    if (r && iter == end) {
        std::cout << "Parse Success!!!" << std::endl;
        boost::apply_visitor(json::ast::Printer(0), root);
    } else {
        std::cout << "Failed!!!" << std::endl;
    }
   
} 


