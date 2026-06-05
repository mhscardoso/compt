using namespace std;

template<typename Func, typename... Args>
struct is_callable {
private:
    template<typename F, typename... A>
    static auto test(int) -> decltype(std::declval<F>()(std::declval<A>()...), std::true_type());
    
    template<typename F, typename... A>
    static auto test(...) -> std::false_type;
    
public:
    static constexpr bool value = decltype(test<Func, Args...>(0))::value;
};

// Primary template for Bind class
template<typename Func, typename... BoundArgs>
class Bind {
private:
    Func func;
    std::tuple<BoundArgs...> bound_args;
    
    template<size_t... I, typename... CallArgs>
    auto call_impl(std::index_sequence<I...>, CallArgs&&... call_args) const {
        // For const-correctness, we need to handle const/non-const functors
        // Use const_cast to handle non-const operator() when necessary
        return const_cast<Func&>(func)(std::get<I>(bound_args)..., std::forward<CallArgs>(call_args)...);
    }
    
public:
    Bind(Func f, BoundArgs... args) 
        : func(std::move(f)), bound_args(std::move(args)...) {}
    
    // Operator() that returns either:
    // 1. The result of calling the function if all arguments are provided
    // 2. A new Bind object if more arguments need to be bound
    template<typename... CallArgs>
    auto operator()(CallArgs&&... call_args) const {
        // If we can call the function with all arguments (bound + new)
        if constexpr (is_callable<Func&, BoundArgs..., CallArgs...>::value) {
            return call_impl(std::index_sequence_for<BoundArgs...>{}, 
                           std::forward<CallArgs>(call_args)...);
        }
        // Otherwise, create a new Bind with the additional arguments
        else {
            return Bind<Func, BoundArgs..., std::decay_t<CallArgs>...>(
                func, 
                std::get<BoundArgs>(bound_args)..., 
                std::forward<CallArgs>(call_args)...
            );
        }
    }
};

// Helper function to deduce types automatically
template<typename Func, typename... Args>
auto bind(Func&& func, Args&&... args) {
    return Bind<std::decay_t<Func>, std::decay_t<Args>...>(
        std::forward<Func>(func), 
        std::forward<Args>(args)...
    );
}
