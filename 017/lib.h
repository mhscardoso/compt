#ifndef LIB_H
#define LIB_H

#include <tuple>
#include <functional>
#include <type_traits>
#include <utility>
#include <algorithm>
#include <array>

using namespace std;

// ---------------------------------------------------------------------
// PlaceHolder
// ---------------------------------------------------------------------
struct PlaceHolder {};
inline PlaceHolder __;

// Renamed to avoid collision with std::is_placeholder_v from <functional>
template<typename T>
constexpr bool is_ph_v = std::is_same_v<std::decay_t<T>, PlaceHolder>;

template<typename... Ts>
constexpr bool contains_ph_v = (is_ph_v<Ts> || ...);

// ---------------------------------------------------------------------
// is_callable
// ---------------------------------------------------------------------
template<typename Func, typename... Args>
struct is_callable {
private:
    template<typename F, typename... A>
    static auto test(int) -> decltype(invoke(declval<F>(), declval<A>()...), true_type());

    template<typename F, typename... A>
    static auto test(...) -> false_type;

public:
    static constexpr bool value = decltype(test<Func, Args...>(0))::value;
};

// ---------------------------------------------------------------------
// tuple utilities
// ---------------------------------------------------------------------
template<typename T0, typename... Ts>
auto tuple_tail(const std::tuple<T0, Ts...>& t) {
    return std::apply([](const T0&, const Ts&... rest) { return std::make_tuple(rest...); }, t);
}

// forward declarations
template<typename... Calls>
auto merge(std::tuple<>, std::tuple<Calls...> call_t);

template<typename Old0, typename... Olds>
auto merge(std::tuple<Old0, Olds...> old_t, std::tuple<>);

template<typename Old0, typename... Olds, typename Call0, typename... Calls>
auto merge(std::tuple<Old0, Olds...> old_t, std::tuple<Call0, Calls...> call_t);

template<typename... Calls>
auto merge(std::tuple<>, std::tuple<Calls...> call_t) {
    return call_t;
}

template<typename Old0, typename... Olds>
auto merge(std::tuple<Old0, Olds...> old_t, std::tuple<>) {
    return old_t;
}

template<typename Old0, typename... Olds, typename Call0, typename... Calls>
auto merge(std::tuple<Old0, Olds...> old_t, std::tuple<Call0, Calls...> call_t) {
    if constexpr (is_ph_v<Old0>) {
        auto rest = merge(tuple_tail(old_t), tuple_tail(call_t));
        return std::tuple_cat(std::make_tuple(std::get<0>(call_t)), rest);
    } else {
        auto rest = merge(tuple_tail(old_t), call_t);
        return std::tuple_cat(std::make_tuple(std::get<0>(old_t)), rest);
    }
}

// Compute index of last concrete (non-placeholder) element
template<typename... Ts>
constexpr long long last_concrete_index() {
    bool flags[] = { is_ph_v<Ts>..., false };
    long long result = -1;
    for (size_t i = 0; i < sizeof...(Ts); i++)
        if (!flags[i]) result = (long long)i;
    return result;
}

template<typename Tup, size_t... I>
auto truncate_to_impl(const Tup& t, std::index_sequence<I...>) {
    return std::make_tuple(std::get<I>(t)...);
}

template<typename... Ts>
auto truncate_trailing(std::tuple<Ts...> t) {
    constexpr long long last = last_concrete_index<Ts...>();
    if constexpr (last < 0) {
        return std::tuple<>();
    } else {
        return truncate_to_impl(t, std::make_index_sequence<(size_t)last + 1>{});
    }
}

// ---------------------------------------------------------------------
// Bind
// ---------------------------------------------------------------------
struct from_tuple_t {};
inline constexpr from_tuple_t from_tuple{};

template<typename Func, typename... BoundArgs>
class Bind {
private:
    Func func;
    std::tuple<BoundArgs...> bound_args;

    template<typename... Ts>
    auto finish(std::tuple<Ts...> t) const {
        if constexpr (contains_ph_v<Ts...>) {
            return Bind<Func, Ts...>(func, std::move(t), from_tuple);
        } else if constexpr (is_callable<Func&, Ts...>::value) {
            return std::apply(
                [this](auto&&... args) -> decltype(auto) {
                    return std::invoke(const_cast<Func&>(func),
                                       std::forward<decltype(args)>(args)...);
                }, t);
        } else {
            return Bind<Func, Ts...>(func, std::move(t), from_tuple);
        }
    }

public:
    Bind(Func f, BoundArgs... args)
        : func(std::move(f)), bound_args(std::move(args)...) {}

    Bind(Func f, std::tuple<BoundArgs...> t, from_tuple_t)
        : func(std::move(f)), bound_args(std::move(t)) {}

    template<typename... CallArgs>
    auto operator()(CallArgs... call_args) const {
        auto call_t  = std::make_tuple(call_args...);
        auto merged  = merge(bound_args, call_t);
        auto trimmed = truncate_trailing(merged);
        return finish(trimmed);
    }
};

template<typename Func, typename... Ts>
auto make_bind_impl(Func func, std::tuple<Ts...> t) {
    return Bind<Func, Ts...>(std::move(func), std::move(t), from_tuple);
}

template<typename Func, typename... Args>
auto bind(Func&& func, Args... args) {
    auto t = truncate_trailing(std::make_tuple(args...));
    return make_bind_impl(std::decay_t<Func>(std::forward<Func>(func)), t);
}

#endif // LIB_H
