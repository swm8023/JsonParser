#ifndef _JSON_PARSER_HPP
#define _JSON_PARSER_HPP

#include <string>
#include <map>
#include <vector>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>


namespace json { namespace parser {
    namespace x3 = boost::spirit::x3;

    namespace ast {
        struct value;
        typedef std::map<std::string, value> object;
        typedef std::vector<value> array;
        struct value : x3::variant<std::string, int, float, bool, object, array> {
            using base_type::base_type;
            using base_type::operator=;
        };
        typedef std::pair<std::string, value> key_value_pair;

        struct value_root: x3::position_tagged {
            value entries;
        };
    }

    
    // rule ids
    struct _value_cont_class;
    struct _value_class;
    
    // rules
    x3::rule<_value_cont_class, ast::value_root> const _value_root = "_value_root";
    x3::rule<_value_class, ast::value> const _value = "_value";
    

    // grammer
    auto const _value_def = 


    BOOST_SPIRIT_DEFINE

    

    
}}


BOOST_FUSION_ADAPT_STRUCT(json::parser::ast::value_root, entries)

#endif // _JSON_PARSER_HPP