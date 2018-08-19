#ifndef _JSON_PARSER_HPP
#define _JSON_PARSER_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>

namespace json {
    namespace x3 = boost::spirit::x3;

    namespace ast {
        struct Value;

        struct Null {};
        using Object = std::map<std::string, Value>;
        using Array = std::vector<Value>;
        
        using ObjectMember = std::pair<std::string, Value>;

        struct Value : x3::variant<Null, bool, int, double, std::string, Object, Array> {
            using base_type::base_type;
            using base_type::operator=;
        };

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

    }
    
    namespace grammer {
        x3::rule<struct ValueClass, ast::Value> const value{"value"};
        x3::rule<struct ObjectClass, ast::Object> const object{"object"};
        x3::rule<struct ArrayClass, ast::Array> const array{"array"};
        x3::rule<struct ObjectMemberClass, ast::ObjectMember> const object_member{"object_member"};

        x3::ascii::bool_type const bool_value; 

        auto const null_value = 
            x3::lit("null") >> x3::attr(ast::Null{});

        auto const quote_string = 
            x3::lexeme['"' >> *(x3::char_ - '"') >> '"'];

        auto const value_def = 
            null_value | bool_value | x3::int_ | x3::double_ | quote_string | object | array;

        auto const object_member_def = 
            quote_string >> ':' >> value;
        
        auto const object_def = 
            x3::lit('{') >>  -(object_member % ',') >> x3::lit('}');
    
        auto const array_def =
            x3::lit('[') >>  -(value % ',') >> x3::lit(']');

        BOOST_SPIRIT_DEFINE(value, object, array, object_member)
    }

}

#endif // _JSON_PARSER_HPP