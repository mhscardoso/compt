#include <functional>

using namespace std;

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

template<typename Func, typename... BoundArgs>
class Bind {
private:
    Func func;
    tuple<BoundArgs...> bound_args;
    
    template<size_t... I, typename... CallArgs>
    auto call_impl(index_sequence<I...>, CallArgs&&... call_args) const {
        return invoke(const_cast<Func&>(func),
                           get<I>(bound_args)...,
                           forward<CallArgs>(call_args)...);
    }

    template<size_t... I, typename... CallArgs>
    auto bind_more(index_sequence<I...>, CallArgs&&... call_args) const {
        return Bind<Func, BoundArgs..., decay_t<CallArgs>...>(
            func,
            get<I>(bound_args)...,
            forward<CallArgs>(call_args)...
        );
    }
    
public:
    Bind(Func f, BoundArgs... args) 
        : func(move(f)), bound_args(move(args)...) {}
    
    template<typename... CallArgs>
    auto operator()(CallArgs&&... call_args) const {
        if constexpr (is_callable<Func&, BoundArgs..., CallArgs...>::value) {
            return call_impl(index_sequence_for<BoundArgs...>{}, 
                             forward<CallArgs>(call_args)...);
        } else {
            return bind_more(index_sequence_for<BoundArgs...>{},
                             forward<CallArgs>(call_args)...);
        }
    }
};

template<typename Func, typename... Args>
auto bind(Func&& func, Args&&... args) {
    return Bind<decay_t<Func>, decay_t<Args>...>(
        forward<Func>(func), 
        forward<Args>(args)...
    );
}
