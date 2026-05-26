#include <variant>

using namespace std;

template <size_t I = 0, typename... Ts>
void print_variant(ostream& os, const variant<Ts...>& v) {

    if constexpr (I < sizeof...(Ts)) {

        using T = variant_alternative_t<I, variant<Ts...>>;

        if (holds_alternative<T>(v)) {
            os << get<T>(v);
        }
        else {
            print_variant<I + 1>(os, v);
        }
    }
}


template <size_t I = 0, typename... Ts>
enable_if_t<I == sizeof...(Ts)>
print_variant(ostream&, const variant<Ts...>&) {}


template <typename T, typename... Ts>
ostream& operator<<(ostream& os, variant<T, Ts...>& v) {
    print_variant(os, v);

    return os;
}
