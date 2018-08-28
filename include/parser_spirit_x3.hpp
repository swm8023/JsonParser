#ifndef _JSON_PARSER_SPIRIT_X3_HPP
#define _JSON_PARSER_SPIRIT_X3_HPP

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/io.hpp>

#include "value.hpp"

namespace json { namespace parser {
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
            null_value | bool_value | x3::lexeme[x3::int_ >> !x3::char_(".eE")] | x3::double_ | quote_string | object | array;

        auto const object_member_def = 
            quote_string >> ':' >> value;
        
        auto const object_def = 
            x3::lit('{') >>  -(object_member % ',') >> x3::lit('}');
    
        auto const array_def =
            x3::lit('[') >>  -(value % ',') >> x3::lit(']');

        BOOST_SPIRIT_DEFINE(value, object, array, object_member)
    }

    // Convert to standard value
    struct Converter {
        Converter(Value &rv) : rv(rv) {}

        void operator()(ast::Object const& v) const {
            rv = object_t{};
            for (auto const& item : v) {
                Value val(object_t{});
                boost::apply_visitor(Converter(val), item.second);
                rv[item.first] = val;
            }
        }

        void operator()(ast::Array const& v) const {
            rv = array_t{};
            for (auto const& item : v) {
                Value val;
                boost::apply_visitor(Converter(val), item);
                rv.Append(val);
            }
        }

        void operator()(ast::Null const& v) const {
            rv = null_t{};
        }

        template<typename T>
        void operator()(T const& v) const {
            rv = v;
        }

        Value &rv;
    };


    inline bool Parse(std::string const& str, Value& value) {
        ast::Value root;
        auto iter = str.begin();
        auto end = str.end();
        bool r = x3::phrase_parse(iter, end, grammer::value, x3::ascii::space, root);
        if (r && iter == end) {
            boost::apply_visitor(Converter(value), root);
            return true;
        }
        return false;
    }
}}

#endif // _JSON_PARSER_SPIRIT_X3_HPP