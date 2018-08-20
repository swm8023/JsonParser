#include "json.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
    using namespace json;
    Value v = array_t{
        1, 
        3.5, 
        "abc", 
        array_t{4, 5}, 
        object_t{
            {"a", 1}, 
            {"b", false},
            {"c", array_t{4, 5, "ccc"}}
        }
    };

    v[6] = object_t{
        {"hello", array_t{"world", "!"}},
        {"ni hao", "shi jie!"}
    };

    v[4]["a"] = array_t{2, 3, 4 ,5};
    v[4].Remove("b");
    v.Remove(1);

    std::cout << v << std::endl;
    
}