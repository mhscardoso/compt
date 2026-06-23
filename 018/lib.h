#include <cmath>
#include <functional>

using namespace std;


template <typename E, typename DX>
struct Expr {
    E e;
    DX dx;

    template<typename K>
    auto operator()(K v) const { return e(v); }

    auto operator[](int idx) const {

        auto self = *this;

        return ::Expr{               // <-- qualificado com ::
            [self, idx](const auto& v) {
                return self(v)[idx];
            },
            [](const auto&) {
                return 0;
            }
        };
    }
};


template <typename E, typename DX>
Expr(E, DX) -> Expr<E, DX>;


Expr x{
    [](const auto& v) { return v; },
    [](const auto&)   { return 1; }
};


// ----------------------------------------- //
// ---------- FUNCTORS OPERATIONS ---------- //
// ----------------------------------------- //


template <typename A, typename B>
struct AddEval {
    A a;
    B b;

    auto operator()(double v) const {
        return a(v) + b(v);
    }
};


template <typename A, typename B>
struct AddDx {
    A a;
    B b;

    double operator()(double v) const {
        return a.dx(v) + b.dx(v);
    }
};


template <typename A, typename B>
struct SubEval {
    A a;
    B b;

    double operator()(double v) const {
        return a(v) - b(v);
    }
};


template <typename A, typename B>
struct SubDx {
    A a;
    B b;

    double operator()(double v) const {
        return a.dx(v) - b.dx(v);
    }
};


template <typename A, typename B>
struct DivEval {
    A a;
    B b;

    double operator()(double v) const {
        return a(v) / b(v);
    }
};


template <typename A, typename B>
struct DivDx {
    A a;
    B b;

    double operator()(double v) const {
        return (a.dx(v) * b(v) - a(v) * b.dx(v)) / (b(v) * b(v));
    }
};


template <typename A, typename B>
struct MulEval {
    A a;
    B b;

    double operator()(double v) const {
        return a(v) * b(v);
    }
};


template <typename A, typename B>
struct MulDx {
    A a;
    B b;

    double operator()(double v) const {
        return a.dx(v) * b(v) + a(v) * b.dx(v);
    }
};


// ------------------------------------- //
// ----------   EXPR RETURN   ---------- //
// ------------------------------------- //


template <typename T>
auto adapter(T f) {
    return f;
}

auto adapter(int v) {
    return Expr{
        [v](double) { return v; },
        [] (double) { return 0; }
    };
}

auto adapter(double v) {
    return Expr{
        [v](double) { return v; },
        [] (double) { return 0; }
    };
}

auto adapter(string v) {
    return Expr{
        [v](double) { return v; },
        [] (double) { return 0; }
    };
}


// ------------------------------------- //
// ---------- MATH OPERATIONS ---------- //
// ------------------------------------- //

// template <typename A, typename B>
// auto operator+(A a, B b) {

//     auto aa = adapter(a);
//     auto bb = adapter(b);

//     return Expr{
//         AddEval<decltype(aa), decltype(bb)>{aa, bb},
//         AddDx<decltype(aa), decltype(bb)>{aa, bb}
//     };
// }


template <typename E, typename DX, typename T>
auto operator+(Expr<E, DX> e, T n) {
    return Expr{
        [e, n](const auto& v) {
            return e(v) + n;   // aqui resolve em tempo de instanciação: vector<int>+int, double+int, etc.
        },
        [](const auto&) {
            return 0;          // não há derivada significativa nesse contexto
        }
    };
}


template <typename A, typename B>
auto operator-(A a, B b) {
    auto aa = adapter(a);
    auto bb = adapter(b);

    return Expr{
        SubEval<decltype(aa), decltype(bb)>{aa, bb},
        SubDx<decltype(aa), decltype(bb)>{aa, bb}
    };
}


template <typename A, typename B>
auto operator*(A a, B b) {
    auto aa = adapter(a);
    auto bb = adapter(b);

    return Expr{
        MulEval<decltype(aa), decltype(bb)>{aa, bb},
        MulDx<decltype(aa), decltype(bb)>{aa, bb}
    };
}


template <typename A, typename B>
auto operator/(A a, B b) {
    auto aa = adapter(a);
    auto bb = adapter(b);

    return Expr{
        DivEval<decltype(aa), decltype(bb)>{aa, bb},
        DivDx<decltype(aa), decltype(bb)>{aa, bb}
    };
}


// ---------------------------------------- //
// ---------- COMPOSE OPERATIONS ---------- //
// ---------------------------------------- //


template <typename A>
struct SinEval {
    A a;

    double operator()(double v) const {
        return std::sin(a(v));
    }
};


template <typename A>
struct SinDx {
    A a;

    double operator()(double v) const {
        return std::cos(a(v)) * a.dx(v);
    }
};


template <typename A>
auto sin(A a) {

    auto aa = adapter(a);

    return Expr{
        SinEval<decltype(aa)>{aa},
        SinDx<decltype(aa)>{aa}
    };
}


template <typename A>
struct CosEval {
    A a;

    double operator()(double v) const {
        return std::cos(a(v));
    }
};


template <typename A>
struct CosDx {
    A a;

    double operator()(double v) const {
        return - std::sin(a(v)) * a.dx(v);
    }
};


template <typename A>
auto cos(A a) {

    auto aa = adapter(a);

    return Expr{
        CosEval<decltype(aa)>{aa},
        CosDx<decltype(aa)>{aa}
    };
}



template <typename A>
struct ExpEval {
    A a;

    double operator()(double v) const {
        return std::exp(a(v));
    }
};


template <typename A>
struct ExpDx {
    A a;

    double operator()(double v) const {
        return std::exp(a(v)) * a.dx(v);
    }
};


template <typename A>
auto exp(A a) {

    auto aa = adapter(a);

    return Expr{
        ExpEval<decltype(aa)>{aa},
        ExpDx<decltype(aa)>{aa}
    };
}


template <typename A>
struct LogEval {
    A a;

    double operator()(double v) const {
        return std::log(a(v));
    }
};


template <typename A>
struct LogDx {
    A a;

    double operator()(double v) const {
        return a.dx(v) / a(v);
    }
};


template <typename A>
auto log(A a) {

    auto aa = adapter(a);

    return Expr{
        LogEval<decltype(aa)>{aa},
        LogDx<decltype(aa)>{aa}
    };
}



template <typename A, typename B>
struct PowEval {
    A a;
    B b;

    double operator()(double v) const {
        return std::pow(a(v), b(v));
    }
};


template <typename A, typename B>
struct PowDx {
    A a;
    B b;

    double operator()(double v) const {
        return b(v) * pow(a(v), b(v) - 1) * a.dx(v);
    }
};



template <typename A, typename B>
auto operator->*(A a, B b) {

    static_assert(
        is_integral_v<B>,
        "Operador de potenciação definido apenas para inteiros"
    );

    auto aa = adapter(a);
    auto bb = adapter(b);

    return Expr{
        PowEval<decltype(aa), decltype(bb)>{aa, bb},
        PowDx<decltype(aa), decltype(bb)>{aa, bb}
    };
}


template <typename Src, typename F>
struct FilterStream {
    Src src;
    F f;

    using SrcIter = decltype(std::begin(std::declval<Src&>()));
    using SrcEnd  = decltype(std::end(std::declval<Src&>()));

    struct iterator {
        SrcIter it;
        SrcEnd  last;
        F* pred;

        void skip() {
            while (it != last && !(*pred)(*it)) {
                ++it;
            }
        }

        iterator& operator++() {
            ++it;
            skip();
            return *this;
        }

        decltype(auto) operator*() const { return *it; }

        bool operator!=(const iterator& other) const {
            return it != other.it;
        }
    };

    iterator begin() {
        iterator b{ std::begin(src), std::end(src), &f };
        b.skip();
        return b;
    }

    iterator end() {
        auto e = std::end(src);
        return iterator{ e, e, &f };
    }
};


template <typename Src, typename F>
struct MapStream {
    Src src;
    F f;

    using SrcIter = decltype(std::begin(std::declval<Src&>()));
    using SrcEnd  = decltype(std::end(std::declval<Src&>()));

    struct iterator {
        SrcIter it;
        F* fn;

        iterator& operator++() {
            ++it;
            return *this;
        }

        auto operator*() const { return (*fn)(*it); }

        bool operator!=(const iterator& other) const {
            return it != other.it;
        }
    };

    iterator begin() { return iterator{ std::begin(src), &f }; }
    iterator end()   { return iterator{ std::end(src),   &f }; }
};


// Marcador de "pegue apenas os n primeiros elementos". Usado como o
// lado direito do operador "|" -- eh tratado de forma especial (nao
// eh chamado como funcao).
struct NPrimeiros {
    int n;
    explicit NPrimeiros(int n_) : n(n_) {}
};

template <typename Src>
struct TakeStream {
    Src src;
    int n;

    using SrcIter = decltype(std::begin(std::declval<Src&>()));
    using SrcEnd  = decltype(std::end(std::declval<Src&>()));

    struct iterator {
        SrcIter it;
        SrcEnd  last;
        int     remaining;

        iterator& operator++() {
            ++it;
            --remaining;
            return *this;
        }

        decltype(auto) operator*() const { return *it; }

        // Para assim que "remaining" chega a zero, mesmo que a fonte
        // continue podendo gerar elementos (sequencia infinita).
        bool operator!=(const iterator&) const {
            return remaining > 0 && it != last;
        }
    };

    iterator begin() { return iterator{ std::begin(src), std::end(src), n }; }
    iterator end()   { return iterator{ std::end(src),   std::end(src), 0 }; }
};

// ============================================================ //
// ------------------------  INTERVALO  --------------------------//
// ============================================================ //

// Fonte lazy de inteiros consecutivos.
//   Intervalo(a, b) -> gera a, a+1, ..., b-1   (fim exclusivo)
//   Intervalo(a)    -> gera a, a+1, a+2, ...   (infinito)
struct Intervalo {
    int start;
    int finish;     // ignorado quando infinite == true
    bool infinite;

    Intervalo(int s, int e) : start(s), finish(e), infinite(false) {}
    explicit Intervalo(int s) : start(s), finish(0), infinite(true) {}

    struct iterator {
        int  val;
        bool infinite;

        iterator& operator++() {
            ++val;
            return *this;
        }

        int operator*() const { return val; }

        bool operator!=(const iterator& other) const {
            if (infinite) return true;     // nunca chega ao fim sozinho
            return val != other.val;
        }
    };

    iterator begin() const { return iterator{ start, infinite }; }
    iterator end()   const { return iterator{ finish, infinite }; }
};


template <typename Src, typename F>
auto operator|(Src&& src, F f) {

    if constexpr (is_same_v<std::decay_t<F>, NPrimeiros>) {
        return TakeStream<Src>{ std::forward<Src>(src), f.n };
    }
    else {
        using It   = decltype(std::begin(std::declval<Src&>()));
        using Elem = decltype(*std::declval<It&>());
        using Ret  = decltype(std::declval<F&>()(std::declval<Elem>()));

        if constexpr (is_same_v<Ret, void>) {
            // sink: consome agora mesmo, dispara toda a cadeia lazy
            for (auto it = std::begin(src); it != std::end(src); ++it) {
                f(*it);
            }
            return;
        }
        else if constexpr (is_same_v<Ret, bool>) {
            return FilterStream<Src, F>{ std::forward<Src>(src), std::move(f) };
        }
        else {
            return MapStream<Src, F>{ std::forward<Src>(src), std::move(f) };
        }
    }
}


bool testa_se_e_primo(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}




template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) os << ' ';
        os << v[i];
    }
    return os;
}


template<typename A>
struct PrintProxy {
    ostream& os;
    A e;
    string suffix;

    template <typename T>
    void operator()(const T& v) const {
        os << e(v) << suffix;
    }
};


template <typename E, typename DX>
auto operator<<(ostream& os, Expr<E, DX> e) {
    return PrintProxy<Expr<E, DX>>{ os, e, "" };
}


template <typename A>
auto operator<<(PrintProxy<A> p, char c) {
    p.suffix += c;
    
    return p;
}


template <typename A>
auto operator<<(PrintProxy<A> p, const char* s) {
    p.suffix += s;
    
    return p;
}


template<typename E1, typename DX1,
         typename E2>
auto operator|(
    Expr<E1, DX1> lhs,
    PrintProxy<E2> rhs
) {
    return PrintProxy{
        rhs.os,
        lhs,
        rhs.suffix
    };
}


template <typename A>
struct FilterProxy {
    A e;
    int div;
    int comp;

    bool operator()(double v) const {
        return ((int) e(v)) % div == comp;
    }
};


template <typename E, typename DX>
auto operator%(Expr<E, DX> e, int div) {
    return FilterProxy<Expr<E, DX>>{ e, div, 0 };
}

template <typename A>
auto operator==(FilterProxy<A> f, int comp) {
    f.comp = comp;

    return f;
}


template <typename A, typename T>
struct NeqProxy {
    A e;
    T comp;

    template <typename K>
    bool operator()(const K& v) const {
        return e(v) != comp;
    }
};

template <typename E, typename DX, typename T>
auto operator!=(Expr<E, DX> e, T comp) {
    return NeqProxy<Expr<E, DX>, T>{ e, comp };
}
