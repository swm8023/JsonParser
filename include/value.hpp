#ifndef _JSON_VALUE_HPP
#define _JSON_VALUE_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <type_traits>
#include <variant>
#include <stdexcept>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace json {
    struct null_t {};
    using bool_t = bool;
    using int_t = int;
    using string_t = std::string;
    using double_t = double;

    class Value;
    using object_t = std::map<std::string, Value>;
    using array_t = std::vector<Value>;

    enum class ValueType {
        TYPE_NONE = 0,
        TYPE_BOOL,
        TYPE_INT,
        TYPE_STRING,
        TYPE_DOUBLE,
        TYPE_OBJECT,
        TYPE_ARRAY,
    };
    
    class Value {
    public:
        using var_t = std::variant<null_t, bool_t, int_t, string_t, double_t, object_t, array_t>;
       
        template<typename T>
        using non_self_t = std::enable_if_t<!(std::is_base_of_v<Value, std::remove_reference_t<T>>)>;
        
        // Constructors
        Value(null_t val = null_t()) : val(val) {}
        Value(char const* s): Value(std::string(s)) {}
         
        template <typename T, typename = non_self_t<T>> Value(T const& rhs) : val(rhs) {}
        template <typename T, typename = non_self_t<T>> Value(T&& rhs) : val(std::forward<T>(rhs)) {}

        Value(Value const& rhs) : val(rhs.val) {}
        Value(Value& rhs) : val(rhs.val) {}
        Value(Value&& rhs) : val(std::move(rhs.val)) {}

        template <typename T, typename = non_self_t<T>>
        Value& operator=(T const& rhs) {
            val = rhs;
            return *this;
        }

        template <typename T, typename = non_self_t<T>>
        Value& operator=(T&& rhs) {
            val = std::forward<T>(rhs);
            return *this;
        }

        Value& operator=(Value const& rhs) {
            val = rhs.get();
            return *this;
        }

        Value& operator=(Value&& rhs) {
            val = std::move(rhs.get());
            return *this;
        }

        // Functions
        size_t Size() {
            return std::visit(overloaded{
                [](auto const &arg) -> size_t {return 0;},
                [](object_t const &arg) -> size_t {return arg.size();},
                [](array_t const &arg) -> size_t {return arg.size();}
            }, val);
        }

        ValueType Type() {
            return std::visit(overloaded{
                [](null_t &)   {return ValueType::TYPE_NONE; },
                [](bool_t &)   {return ValueType::TYPE_BOOL; },
                [](int_t &)    {return ValueType::TYPE_INT; },
                [](string_t &) {return ValueType::TYPE_STRING; },
                [](double_t &) {return ValueType::TYPE_DOUBLE; },
                [](object_t &) {return ValueType::TYPE_OBJECT; },
                [](array_t &)  {return ValueType::TYPE_ARRAY; }
            }, val);
        }

        // Array
        Value const& operator[](int index) const {
            auto &arr = std::get<array_t>(val);
            return arr.at(index);
        }

        Value& operator[](int index) {
            auto &arr = std::get<array_t>(val);
            if (arr.size() <= (size_t)index)
                arr.resize(index + 1, null_t{});
            return arr.at(index);
        }

        void Append(Value const& v) {
            auto &arr = std::get<array_t>(val);
            arr.push_back(v);
        }

        void Remove(int index) {
            auto &arr = std::get<array_t>(val);
            if (index < arr.size()) arr.erase(arr.begin() + index);
        }

        // Object
        Value const& operator[](std::string const& s) const {
            auto &obj = std::get<object_t>(val);
            auto iter = obj.find(s);
            if (iter == obj.end()) throw std::out_of_range("object item not exist");
            return iter->second;
        }

        Value& operator[](std::string const& s) {
            auto &obj = std::get<object_t>(val);
            return obj[s];
        }

        Value const& operator[](const char *s) const {
            return (*this)[std::string(s)];
        }

        Value& operator[](const char *s) {
            return (*this)[std::string(s)];
        }

        void Remove(std::string const& s) {
            auto &obj = std::get<object_t>(val);
            auto iter = obj.find(s);
            if (iter != obj.end()) obj.erase(iter);
        }

        // Getter with No expection
        bool GetValSafety(double &t) {
            try {
                t = std::get<double>(val);
            } catch(...) {
                if (Type() == ValueType::TYPE_INT) {
                    t = (double)std::get<int>(val);
                    return true;
                }
                return false;
            }
            return false;
        } 

        template<typename T>
        bool GetValSafety(std::vector<T> &vec) {
            if (Type() != ValueType::TYPE_ARRAY) {
                return false;
            }
            auto &arr = std::get<array_t>(val);
            for (auto &item : arr) {
                T v;
                if (!item.GetValSafety(v)) {
                    vec.clear();
                    return false;
                }
                vec.push_back(v);
            }
            return true;
        }
        
        bool GetValSafety(array_t &vec) {
            vec = std::get<array_t>(val);
            return true;
        }

        template<typename T>
        bool GetValSafety(T &t) {
            try {
                t = std::get<T>(val);
            } catch(...) {
                return false;
            }
            return true;
        }

        template<typename T, typename FirstArg, typename... Args>
        bool GetValSafety(T &t, FirstArg const& first_arg, Args const&... other_args) {
            try {
                Value &v = (*this)[first_arg];
                return v.GetValSafety(t, other_args...);
            } catch(...) {
                return false;
            }  
        }

        // get original variant
        var_t const& get() const {
            return val;
        }

        var_t& get() {
            return val;
        }

    private:
        var_t val;

    };

    // Printer
    struct Printer {
        Printer(std::ostream& os, int tab_num=0, bool is_last=true, bool format=true) :
            os(os), tab(tab_num), is_last(is_last), format(format) {}

        void operator()(object_t const& v) const {
            os << '{' << end();
            int idx = 0;
            for (auto const& item : v) {
                idx += 1;
                os << tabstr(tab + 1) << '"' << item.first << "\" : ";
                std::visit(Printer(os, tab + 1, idx == v.size(), format), item.second.get());
            }
            os << tabstr(tab) << '}' << laststr() << end();
        }

        void operator()(array_t const& v) const {
            os << '[' << end();
            int idx = 0;
            for (auto const& item : v) {
                idx += 1;
                os << tabstr(tab + 1);
                std::visit(Printer(os, tab + 1, idx == v.size(), format), item.get());
            }
            os << tabstr(tab) << ']' << laststr() << end();
        }

        void operator()(null_t const& v) const {
            os << "null" << laststr() << end();
        }

        void operator()(string_t const& v) const {
            os << '"' << v << '"' << laststr() << end();
        }

        void operator()(bool_t const& v) const {
            os << (v ? "true" : "false") << laststr() << end();
        }

        template<typename T>
        void operator()(T const& v) const {
            os << v << laststr() << end();
        }

        std::string tabstr(int i, int tab_sz = 4) const {
            return format ? std::string(i * tab_sz, ' ') : "";
        }
        
        std::string end() const {
            return format ? "\n" : "";
        }

        std::string laststr() const {
            return std::string(is_last ? "": "," );
        }
        int tab = 0;
        std::ostream &os;
        bool is_last;
        bool format;
    };

    inline std::ostream & operator<<(std::ostream &os, Value const& v){
        std::visit(Printer(os, 0), v.get());
        return os;
    }
}

#endif // _JSON_VALUE_HPP

