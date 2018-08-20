#ifndef _JSON_JSON_HPP
#define _JSON_JSON_HPP

#define JSON_USE_PARSER_SPIRIT

#include "value.hpp"

#ifdef JSON_USE_PARSER_SPIRIT
#include "parser_spirit_x3.hpp"
#else
#include "parser_normal.hpp"
#endif

namespace json {
    inline bool Parse(std::string const& str, Value& value) {
        return parser::Parse(str, value);
    }

}

#endif // _JSON_JSON_HPP