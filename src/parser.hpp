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
        struct value;

        struct null_t {};

        using object = std::map<std::string, value>;
        using array = std::vector<value>;
        
        using object_member = std::pair<std::string, value>;

        struct value : x3::variant<std::string, object, array> {
            using base_type::base_type;
            using base_type::operator=;
        };

        int const tabsize = 4;

        struct value_printer
        {
            typedef void result_type;

            value_printer(int indent = 0) : indent(indent) {}

            void operator()(object const& cont) const
            {
                std::cout << '{' << std::endl;
                for (auto const& entry : cont)
                {
                    tab(indent+tabsize);
                    std::cout << '"' << entry.first << "\" = ";
                    boost::apply_visitor(value_printer(indent+tabsize), entry.second);
                }
                tab(indent);
                std::cout << '}' << std::endl;
            }

            void operator()(std::string const& text) const
            {
                std::cout << '"' << text << '"' << std::endl;
            }

            void tab(int spaces) const
            {
                for (int i = 0; i < spaces; ++i)
                    std::cout << ' ';
            }

            int indent;
        };
    }

    namespace grammer {
        struct value_class;
        struct object_class;
        struct array_class;
        struct object_member_class;

        x3::rule<value_class, ast::value> const value{"value"};
        x3::rule<object_class, ast::object> const object{"object"};
        x3::rule<array_class, ast::array> const array{"array"};
        x3::rule<object_member_class, ast::object_member> const object_member{"object_member"};

        auto const quote_string = 
            x3::lexeme['"' >> *(x3::char_ - '"') >> '"'];

        auto const value_def = 
            quote_string | object | array;

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