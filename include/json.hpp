#ifndef _JSON_JSON_HPP
#define _JSON_JSON_HPP


#include <fstream>

#include "config.h"
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

    inline bool ParseFile(std::string fpath, Value& value) {
        std::ifstream st(fpath);
        if (!st.is_open()) {
            return false;
        }
        std::string str((std::istreambuf_iterator<char>(st)), std::istreambuf_iterator<char>());
        return Parse(str, value);
    }

}

#endif // _JSON_JSON_HPP