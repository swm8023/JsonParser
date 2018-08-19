#ifndef _JSON_PARSER_HPP
#define _JSON_PARSER_HPP

#include <string>
#include <map>
#include <vector>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>


namespace json { namespace ast {
    namespace x3 = boost::spirit::x3;

    // struct value;
    // typedef std::map<std::string, value> object;
    // typedef std::vector<value> array;
    // typedef std::pair<std::string, value> kv_pair;

    struct value : x3::variant<std::string, int> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct value_root {
        value root;
    };

}}

BOOST_FUSION_ADAPT_STRUCT(json::ast::value_root, root)

namespace json {namespace grammer {
    namespace x3 = boost::spirit::x3;    
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;

    using ascii::char_;
    using ascii::string;
    // rule ids
    struct value_root_class {};
    struct value_class {};
    struct object_class {};
    struct array_class {};
    struct kv_pair_class {};
    
    // rules
    x3::rule<value_root_class, ast::value_root> const value_root = "value_root";
    x3::rule<value_class, ast::value> const value = "value";
    // x3::rule<object_class, ast::object> const object = "object";
    // x3::rule<array_class, ast::array> const array = "array";
    // x3::rule<kv_pair_class, ast::kv_pair> const kv_pair = "kv_pair";

    // grammer
    auto const quoted_string = x3::lexeme['"' >> *(x3::char_ - '"') >> '"'];
    auto const value_def = quoted_string;
    // auto const kv_pair_def = quoted_string >> '=' >> value;
    // auto const object_def = '{' >> *kv_pair >> '}';
    auto const value_root_def = value;

    // BOOST_SPIRIT_DEFINE(value_root, value, object, kv_pair);
    BOOST_SPIRIT_DEFINE(value_root, value);

    
}}





#endif // _JSON_PARSER_HPP