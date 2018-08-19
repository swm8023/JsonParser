#include <iostream>
#include <type_traits>
#include <variant>


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace json {

    struct null_t{};
    using bool_t = bool;
    using int_t = int;
    using string_t = std::string;
    using double_t = double;

    class Value;
    using object_t = std::map<std::string, Value>;
    using array_t = std::vector<Value>;

    class Value {
    public:

        using var_t = std::variant<null_t, bool_t, int_t, string_t, double_t, object_t, array_t>;
       
        template<typename T>
        using non_self_t = std::enable_if_t<!(std::is_base_of_v<Value, std::remove_reference_t<T>>)>;
        
        // Constructors
        template <typename T, typename = non_self_t<T>> Value(T const& rhs) : var(rhs) {}
        template <typename T, typename = non_self_t<T>> Value(T&& rhs) : var(std::forward<T>(rhs)) {}

        Value(Value const& rhs) : var(rhs.var) {}
        Value(Value& rhs) : var(rhs.var) {}
        Value(Value&& rhs) : var(std::move(rhs.var)) {}

        template <typename T, typename = non_self_t<T>>
        Value& operator=(T const& rhs) {
            var = rhs;
            return *this;
        }

        template <typename T, typename = non_self_t<T>>
        Value& operator=(T&& rhs) {
            var = std::forward<T>(rhs);
            return *this;
        }

        Value& operator=(Value const& rhs) {
            var = rhs.get();
            return *this;
        }

        Value& operator=(Value&& rhs) {
            var = std::move(rhs.get());
            return *this;
        }

        // Functions
        size_t Size() {
            return std::visit(overloaded{
                [](auto const &arg) -> size_t {return 0;},
                [](object_t const &arg) -> size_t {return arg.size();},
                [](array_t const &arg) -> size_t {return arg.size();}
            }, var);
        }
        // 
        var_t const& get() const {
            return var;
        }

        var_t& get() {
            return var;
        }

        var_t var;

    };

   
}