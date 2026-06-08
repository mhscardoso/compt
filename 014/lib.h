using namespace std;

template <typename E, typename DX>
struct Expr {
    E e;
    DX dx;

    template<typename K>
    auto operator()(K v) const { return e(v); }

    string str()    const { return e.str();  }
    string dx_str() const { return dx.str(); }
};

template <typename E, typename DX>
Expr(E, DX) -> Expr<E, DX>;


// Substituição mínima dos lambdas de x por structs com .str()
struct XEval {
    auto operator()(const auto& v) const { return v; }
    string str() const { return "x"; }
};

struct XDx {
    auto operator()(const auto&) const { return 1; }
    string str() const { return "1"; }
};

Expr x{ XEval{}, XDx{} };


// ----------------------------------------- //
// ---------- FUNCTORS OPERATIONS ---------- //
// ----------------------------------------- //


template <typename A, typename B>
struct AddEval {
    A a;
    B b;

    auto operator()(const auto& v) const {
        return a(v) + b(v);
    }

    string str() const {
        return "((" + a.str() + ")" + "+" + "(" + b.str() + "))";
    }
};


template <typename A, typename B>
struct AddDx {
    A a;
    B b;

    double operator()(double v) const {
        return a.dx(v) + b.dx(v);
    }

    string str() const {
        return "((" + a.dx_str() + ")" + "+" + "(" + b.dx_str() + "))";
    }
};


template <typename A, typename B>
struct SubEval {
    A a;
    B b;

    double operator()(double v) const {
        return a(v) - b(v);
    }

    string str() const {
        return "((" + a.str() + ")" + "-" + "(" + b.str() + "))";
    }
};


template <typename A, typename B>
struct SubDx {
    A a;
    B b;

    double operator()(double v) const {
        return a.dx(v) - b.dx(v);
    }

    string str() const {
        return "((" + a.dx_str() + ")" + "-" + "(" + b.dx_str() + "))";
    }
};


template <typename A, typename B>
struct DivEval {
    A a;
    B b;

    double operator()(double v) const {
        return a(v) / b(v);
    }

    string str() const {
        return "((" + a.str() + ")/(" + b.str() + "))";
    }
};


template <typename A, typename B>
struct DivDx {
    A a;
    B b;

    double operator()(double v) const {
        return (a.dx(v) * b(v) - a(v) * b.dx(v)) / (b(v) * b(v));
    }

    string str() const {
        return "(((" + a.dx_str() + ")*(" + b.str() + ")-(" + a.str() + ")*(" + b.dx_str() + "))/(" + b.str() + ")^2)";
    }
};


template <typename A, typename B>
struct MulEval {
    A a;
    B b;

    double operator()(double v) const {
        return a(v) * b(v);
    }

    string str() const {
        return "((" + a.str() + ")*(" + b.str() + "))";
    }
};


template <typename A, typename B>
struct MulDx {
    A a;
    B b;

    double operator()(double v) const {
        return a.dx(v) * b(v) + a(v) * b.dx(v);
    }

    string str() const {
        return "((" + a.dx_str() + ")*(" + b.str() + ")+(" + a.str() + ")*(" + b.dx_str() + "))";
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
    struct Eval {
        int v;
        double operator()(double) const { return v; }
        string str() const { return to_string(v); }  // sem parênteses
    };
    struct Dx {
        double operator()(double) const { return 0; }
        string str() const { return "0"; }
    };
    return Expr{ Eval{v}, Dx{} };
}

auto adapter(double v) {
struct Eval {
        double v;
        double operator()(double) const { return v; }
        string str() const {
            string s = to_string(v);
            s.erase(s.find_last_not_of('0') + 1);
            s.erase(s.find_last_not_of('.') + 1);
            return s;  // sem parênteses
        }
    };
    struct Dx {
        double operator()(double) const { return 0; }
        string str() const { return "0"; }
    };
    return Expr{ Eval{v}, Dx{} };
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

template <typename A, typename B>
auto operator+(A a, B b) {
    auto aa = adapter(a);
    auto bb = adapter(b);
    return Expr{
        AddEval<decltype(aa), decltype(bb)>{aa, bb},
        AddDx<decltype(aa), decltype(bb)>{aa, bb}
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
    double operator()(double v) const { return std::sin(a(v)); }
    string str() const { return "sin(" + a.str() + ")"; }
};

template <typename A>
struct SinDx {
    A a;
    double operator()(double v) const { return std::cos(a(v)) * a.dx(v); }
    string str() const { return "(cos(" + a.str() + ")*" + a.dx_str() + ")"; }
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
    double operator()(double v) const { return std::cos(a(v)); }
    string str() const { return "cos(" + a.str() + ")"; }
};

template <typename A>
struct CosDx {
    A a;
    double operator()(double v) const { return -std::sin(a(v)) * a.dx(v); }
    string str() const { return "(-sin(" + a.str() + ")*" + a.dx_str() + ")"; }
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
    double operator()(double v) const { return std::exp(a(v)); }
    string str() const { return "exp(" + a.str() + ")"; }
};

template <typename A>
struct ExpDx {
    A a;
    double operator()(double v) const { return std::exp(a(v)) * a.dx(v); }
    string str() const { return "(exp(" + a.str() + ")*" + a.dx_str() + ")"; }
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
    double operator()(double v) const { return std::log(a(v)); }
    string str() const { return "log(" + a.str() + ")"; }
};

template <typename A>
struct LogDx {
    A a;
    double operator()(double v) const { return a.dx(v) / a(v); }
    string str() const {
        return "(1/(" + a.str() + ")*" + a.dx_str() + ")";
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
    double operator()(double v) const { return std::pow(a(v), b(v)); }
    string str() const { return "(" + a.str() + ")^" + b.str(); }
};

template <typename A, typename B>
struct PowDx {
    A a;
    B b;
    double operator()(double v) const {
        return b(v) * pow(a(v), b(v) - 1) * a.dx(v);
    }
    string str() const {
        // b(v) - 1 calculado numericamente, convertido para string
        string exp_menos1 = to_string((int)(b(0) - 1));
        return "(" + b.str() + "*(" + a.dx_str() + ")*(" + a.str() + ")^" + exp_menos1 + ")";
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


template <typename T, typename F>
auto operator|(const T& arr, F func) {

    using Elem = decay_t<decltype(*begin(arr))>;
    using Ret  = decay_t<invoke_result_t<F, Elem>>;

    if constexpr (is_same_v<Ret, void>) {
        for_each(begin(arr), end(arr), func);
        return;
    }
    else if constexpr (is_same_v<Ret, bool>) {
        vector<Elem> result;
        for (const auto& e : arr)
            if (invoke(func, e))
                result.push_back(e);
        return result;
    } else {
        vector<Ret> result;
        for (const auto& e : arr)
            result.push_back(invoke(func, e));
        return result;
    }
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
