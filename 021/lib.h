#pragma once
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <type_traits>
#include <cmath>

using namespace std;

namespace simp {

enum class Kind { Add, Sub, Mul, Div, Pow, Atom, Neg };

struct Term {
    string text;
    Kind   kind;
};

inline bool is_zero(const Term& t) { return t.text == "0"; }
inline bool is_one (const Term& t) { return t.text == "1"; }

inline bool is_add_or_sub(Kind k) { return k == Kind::Add || k == Kind::Sub; }

inline string paren(const Term& t) { return "(" + t.text + ")"; }

// ----- Texto do filho dentro de uma soma (lado esquerdo ou direito) -----
inline string add_operand(const Term& t) {
    // soma nunca precisa de parênteses nos filhos
    return t.text;
}

// ----- Texto do filho direito de uma subtração -----
inline string sub_right_operand(const Term& t) {
    if (is_add_or_sub(t.kind)) return paren(t);
    return t.text;
}

// ----- Texto do filho esquerdo/direito de multiplicação -----
inline string mul_operand(const Term& t) {
    if (is_add_or_sub(t.kind)) return paren(t);
    return t.text;
}

// ----- Texto do filho esquerdo de divisão -----
inline string div_left_operand(const Term& t) {
    if (is_add_or_sub(t.kind)) return paren(t);
    return t.text;
}

// ----- Texto do filho direito de divisão -----
inline string div_right_operand(const Term& t) {
    if (t.kind == Kind::Add || t.kind == Kind::Sub ||
        t.kind == Kind::Mul || t.kind == Kind::Div) {
        return paren(t);
    }
    return t.text;
}

// ----- Texto de base/expoente de potência -----
inline string pow_operand(const Term& t) {
    if (t.kind != Kind::Atom) return paren(t);
    return t.text;
}


// ----- Texto do operando de uma negação prefixa "-x" -----
inline string neg_operand(const Term& t) {
    // soma/sub precisam de parênteses sob um prefixo unário;
    // mul/div/pow/atom já se bastam (ex: -a*b deve ser lido como -(a*b))
    if (is_add_or_sub(t.kind)) return paren(t);
    return t.text;
}

// Constrói o termo de negação de t: "-t" (ou colapsa duplas negações)
inline Term negate(const Term& t) {
    return Term{ "-" + neg_operand(t), Kind::Neg };
}

// ------- Adição -------
inline Term add(const Term& a, const Term& b) {
    if (is_zero(a)) return b;
    if (is_zero(b)) return a;
    return Term{ add_operand(a) + "+" + add_operand(b), Kind::Add };
}

// ------- Subtração -------
inline Term sub(const Term& a, const Term& b) {
    if (is_zero(b)) return a;
    if (is_zero(a)) return negate(b);
    return Term{ a.text + "-" + sub_right_operand(b), Kind::Sub };
}

// ------- Multiplicação -------
inline Term mul(const Term& a, const Term& b) {
    if (is_zero(a) || is_zero(b)) return Term{ "0", Kind::Atom };
    if (is_one(a))  return b;
    if (is_one(b))  return a;
    return Term{ mul_operand(a) + "*" + mul_operand(b), Kind::Mul };
}

// ------- Divisão -------
inline Term div_(const Term& a, const Term& b) {
    return Term{ div_left_operand(a) + "/" + div_right_operand(b), Kind::Div };
}

// ------- Potência -------
inline Term pow_(const Term& base, const Term& exp) {
    if (is_zero(exp)) return Term{ "1", Kind::Atom };
    if (is_one(exp))  return base;
    return Term{ pow_operand(base) + "^" + pow_operand(exp), Kind::Pow };
}

// ------- Derivada da potência: b * a' * a^(b-1) -------
inline Term pow_dx(const Term& base, const Term& exp, const Term& inner_dx) {
    // calcula expoente - 1
    Term exp_m1;
    try {
        int e = stoi(exp.text);
        exp_m1 = Term{ to_string(e - 1), Kind::Atom };
    } catch (...) {
        exp_m1 = Term{ exp.text + "-1", Kind::Sub };
    }
    Term base_pow = pow_(base, exp_m1);      // a^(b-1)
    Term coeff    = mul(exp, inner_dx);      // b * a'
    return mul(coeff, base_pow);             // * a^(b-1)
}

// ------- chain rule genérica: outer * inner_dx -------
inline Term chain(const Term& outer, const Term& inner_dx) {
    return mul(outer, inner_dx);
}

// ------- Derivada de quociente: (f'g - fg') / g^2 -------
inline Term quot_dx(const Term& f, const Term& f_dx,
                     const Term& g, const Term& g_dx) {
    Term num_left  = mul(f_dx, g);
    Term num_right = mul(f, g_dx);
    Term numerator = sub(num_left, num_right);            // f'g - fg'
    Term denominator = pow_(g, Term{"2", Kind::Atom});     // g^2
    return div_(numerator, denominator);
}

// ------- Função (sin/cos/exp/log): sempre átomo (tem parênteses próprios) -------
inline Term func(const string& name, const Term& arg) {
    return Term{ name + "(" + arg.text + ")", Kind::Atom };
}

// ------- Unário "-": prefixo, mantém kind do operando interno para fins de mul -------
// Usado apenas para -sin(x) em CosDx; tratamos como Atom para simplificar (já isolado por func)
inline Term neg_prefix(const Term& t) {
    // Para manter "-sin(x)" colável em chain/mul sem parenteses redundantes,
    // tratamos como Atom (a função já se auto-delimita com "(...)")
    return Term{ "-" + t.text, Kind::Atom };
}

} // namespace simp


// ============================================================
// Estrutura principal — agora str()/dx_str() retornam apenas
// o texto; o Kind fica encapsulado na implementação interna.
// ============================================================

template <typename E, typename DX>
struct Expr {
    E e;
    DX d;

    template<typename K>
    auto operator()(K v) const { return e(v); }

    // Avaliação numérica da derivada (delega ao functor armazenado em `d`)
    double dx(double v) const { return d(v); }

    // term() retorna a representação simplificada desta expressão.
    // dx_term() retorna a representação simplificada de sua derivada.
    simp::Term term()    const { return e.term(); }
    simp::Term dx_term() const { return d.term(); }

    string str()    const { return e.term().text; }
    string dx_str() const { return d.term().text; }
};

template <typename E, typename DX>
Expr(E, DX) -> Expr<E, DX>;


// ============================================================
// FUNCTORS DE AVALIAÇÃO E DERIVAÇÃO
//
// Cada functor expõe term() retornando simp::Term (texto+kind),
// além de operator() para avaliação numérica.
// ============================================================

template <typename A, typename B>
struct AddEval {
    A a; B b;
    double operator()(double v) const { return a(v) + b(v); }
    simp::Term term() const { return simp::add(a.term(), b.term()); }
};

template <typename A, typename B>
struct AddDx {
    A a; B b;
    double operator()(double v) const { return a.dx(v) + b.dx(v); }
    simp::Term term() const { return simp::add(a.dx_term(), b.dx_term()); }
};

template <typename A, typename B>
struct SubEval {
    A a; B b;
    double operator()(double v) const { return a(v) - b(v); }
    simp::Term term() const { return simp::sub(a.term(), b.term()); }
};

template <typename A, typename B>
struct SubDx {
    A a; B b;
    double operator()(double v) const { return a.dx(v) - b.dx(v); }
    simp::Term term() const { return simp::sub(a.dx_term(), b.dx_term()); }
};

template <typename A, typename B>
struct MulEval {
    A a; B b;
    double operator()(double v) const { return a(v) * b(v); }
    simp::Term term() const { return simp::mul(a.term(), b.term()); }
};

template <typename A, typename B>
struct MulDx {
    A a; B b;
    double operator()(double v) const {
        return a.dx(v) * b(v) + a(v) * b.dx(v);
    }
    simp::Term term() const {
        return simp::add(
            simp::mul(a.dx_term(), b.term()),
            simp::mul(a.term(),    b.dx_term())
        );
    }
};

template <typename A, typename B>
struct DivEval {
    A a; B b;
    double operator()(double v) const { return a(v) / b(v); }
    simp::Term term() const { return simp::div_(a.term(), b.term()); }
};

template <typename A, typename B>
struct DivDx {
    A a; B b;
    double operator()(double v) const {
        return (a.dx(v) * b(v) - a(v) * b.dx(v)) / (b(v) * b(v));
    }
    simp::Term term() const {
        return simp::quot_dx(a.term(), a.dx_term(), b.term(), b.dx_term());
    }
};


// ============================================================
// FOLHAS (leaves)
//
// Cada folha expõe term() / dx_term() retornando simp::Term,
// e dx(double) para avaliação numérica da derivada.
// ============================================================

struct XLeaf {
    double operator()(double v) const { return v; }
    simp::Term term()    const { return simp::Term{ "x", simp::Kind::Atom }; }
    simp::Term dx_term() const { return simp::Term{ "1", simp::Kind::Atom }; }
    double dx(double)  const { return 1.0; }
};

struct XDxLeaf {
    double operator()(double) const { return 1.0; }
    simp::Term term()    const { return simp::Term{ "1", simp::Kind::Atom }; }
    simp::Term dx_term() const { return simp::Term{ "0", simp::Kind::Atom }; }
};

struct ConstLeaf {
    double val;
    string repr;
    double operator()(double) const { return val; }
    simp::Term term()    const { return simp::Term{ repr, simp::Kind::Atom }; }
    simp::Term dx_term() const { return simp::Term{ "0", simp::Kind::Atom }; }
    double dx(double)  const { return 0.0; }
};

struct ConstDxLeaf {
    double operator()(double) const { return 0.0; }
    simp::Term term()    const { return simp::Term{ "0", simp::Kind::Atom }; }
    simp::Term dx_term() const { return simp::Term{ "0", simp::Kind::Atom }; }
};


// ============================================================
// ADAPTER
// ============================================================

template <typename T>
auto adapter(T t) { return t; }

inline auto adapter(int v) {
    return Expr{
        ConstLeaf{ (double)v, to_string(v) },
        ConstDxLeaf{}
    };
}

inline auto adapter(double v) {
    string repr = to_string(v);
    if (repr.find('.') != string::npos) {
        repr.erase(repr.find_last_not_of('0') + 1);
        if (repr.back() == '.') repr.pop_back();
    }
    return Expr{
        ConstLeaf{ v, repr },
        ConstDxLeaf{}
    };
}

inline auto adapter(const string& v) {
    struct StrLeaf {
        string val;
        string operator()(double) const { return val; }
        simp::Term term()    const { return simp::Term{ val, simp::Kind::Atom }; }
        simp::Term dx_term() const { return simp::Term{ "0", simp::Kind::Atom }; }
    };
    struct StrDxLeaf {
        simp::Term term()    const { return simp::Term{ "0", simp::Kind::Atom }; }
        simp::Term dx_term() const { return simp::Term{ "0", simp::Kind::Atom }; }
        string operator()(double) const { return "0"; }
    };
    return Expr{ StrLeaf{v}, StrDxLeaf{} };
}

// Variável global x
inline Expr x{ XLeaf{}, XDxLeaf{} };


// ============================================================
// OPERADORES ARITMÉTICOS
// ============================================================

template <typename A, typename B>
auto operator+(A a, B b) {
    auto aa = adapter(a);
    auto bb = adapter(b);
    return Expr{
        AddEval<decltype(aa), decltype(bb)>{aa, bb},
        AddDx <decltype(aa), decltype(bb)>{aa, bb}
    };
}

template <typename A, typename B>
auto operator-(A a, B b) {
    auto aa = adapter(a);
    auto bb = adapter(b);
    return Expr{
        SubEval<decltype(aa), decltype(bb)>{aa, bb},
        SubDx <decltype(aa), decltype(bb)>{aa, bb}
    };
}

template <typename A, typename B>
auto operator*(A a, B b) {
    auto aa = adapter(a);
    auto bb = adapter(b);
    return Expr{
        MulEval<decltype(aa), decltype(bb)>{aa, bb},
        MulDx <decltype(aa), decltype(bb)>{aa, bb}
    };
}

template <typename A, typename B>
auto operator/(A a, B b) {
    auto aa = adapter(a);
    auto bb = adapter(b);
    return Expr{
        DivEval<decltype(aa), decltype(bb)>{aa, bb},
        DivDx <decltype(aa), decltype(bb)>{aa, bb}
    };
}


// ============================================================
// FUNÇÕES COMPOSTAS: sin, cos, exp, log
// ============================================================

template <typename A>
struct SinEval {
    A a;
    double operator()(double v) const { return std::sin(a(v)); }
    simp::Term term() const { return simp::func("sin", a.term()); }
};

template <typename A>
struct SinDx {
    A a;
    double operator()(double v) const { return std::cos(a(v)) * a.dx(v); }
    // cos(f) * f'
    simp::Term term() const {
        return simp::chain(simp::func("cos", a.term()), a.dx_term());
    }
};

template <typename A>
auto sin(A a) {
    auto aa = adapter(a);
    return Expr{ SinEval<decltype(aa)>{aa}, SinDx<decltype(aa)>{aa} };
}

// ----

template <typename A>
struct CosEval {
    A a;
    double operator()(double v) const { return std::cos(a(v)); }
    simp::Term term() const { return simp::func("cos", a.term()); }
};

template <typename A>
struct CosDx {
    A a;
    double operator()(double v) const { return -std::sin(a(v)) * a.dx(v); }
    // -sin(f) * f'
    simp::Term term() const {
        simp::Term neg_sin = simp::neg_prefix(simp::func("sin", a.term()));
        return simp::chain(neg_sin, a.dx_term());
    }
};

template <typename A>
auto cos(A a) {
    auto aa = adapter(a);
    return Expr{ CosEval<decltype(aa)>{aa}, CosDx<decltype(aa)>{aa} };
}

// ----

template <typename A>
struct ExpEval {
    A a;
    double operator()(double v) const { return std::exp(a(v)); }
    simp::Term term() const { return simp::func("exp", a.term()); }
};

template <typename A>
struct ExpDx {
    A a;
    double operator()(double v) const { return std::exp(a(v)) * a.dx(v); }
    // exp(f) * f'
    simp::Term term() const {
        return simp::chain(simp::func("exp", a.term()), a.dx_term());
    }
};

template <typename A>
auto exp(A a) {
    auto aa = adapter(a);
    return Expr{ ExpEval<decltype(aa)>{aa}, ExpDx<decltype(aa)>{aa} };
}

// ----

template <typename A>
struct LogEval {
    A a;
    double operator()(double v) const { return std::log(a(v)); }
    simp::Term term() const { return simp::func("log", a.term()); }
};

template <typename A>
struct LogDx {
    A a;
    double operator()(double v) const { return a.dx(v) / a(v); }
    // (1/f) * f'
    simp::Term term() const {
        simp::Term one = simp::Term{ "1", simp::Kind::Atom };
        simp::Term inv = simp::div_(one, a.term());
        return simp::mul(inv, a.dx_term());
    }
};

template <typename A>
auto log(A a) {
    auto aa = adapter(a);
    return Expr{ LogEval<decltype(aa)>{aa}, LogDx<decltype(aa)>{aa} };
}


// ============================================================
// POTENCIAÇÃO  ->*
// ============================================================

template <typename A, typename B>
struct PowEval {
    A a; B b;
    double operator()(double v) const { return std::pow(a(v), b(v)); }
    simp::Term term() const { return simp::pow_(a.term(), b.term()); }
};

template <typename A, typename B>
struct PowDx {
    A a; B b;
    double operator()(double v) const {
        return b(v) * std::pow(a(v), b(v) - 1) * a.dx(v);
    }
    simp::Term term() const {
        return simp::pow_dx(a.term(), b.term(), a.dx_term());
    }
};

template <typename A>
auto operator->*(A a, int b) {
    auto aa = adapter(a);
    auto bb = adapter(b);
    return Expr{
        PowEval<decltype(aa), decltype(bb)>{aa, bb},
        PowDx <decltype(aa), decltype(bb)>{aa, bb}
    };
}


// ============================================================
// PIPE OPERATOR  |
// ============================================================

template <typename T, typename = void>
struct is_iterable : false_type {};

template <typename T>
struct is_iterable<T, void_t<
    decltype(std::begin(std::declval<const T&>())),
    decltype(std::end(std::declval<const T&>()))
>> : true_type {};

template <typename T, typename F,
          typename = enable_if_t<is_iterable<T>::value>>
auto operator|(const T& arr, F func) {
    using Elem = decay_t<decltype(*std::begin(arr))>;
    using Ret  = decay_t<invoke_result_t<F, Elem>>;

    if constexpr (is_same_v<Ret, void>) {
        for_each(std::begin(arr), std::end(arr), func);
    } else if constexpr (is_same_v<Ret, bool>) {
        vector<Elem> result;
        for (const auto& e : arr)
            if (invoke(func, e)) result.push_back(e);
        return result;
    } else {
        vector<Ret> result;
        for (const auto& e : arr)
            result.push_back(invoke(func, e));
        return result;
    }
}


// ============================================================
// PRINT PROXY  (cout << expr << '\n')
// ============================================================

template<typename A>
struct PrintProxy {
    ostream& os;
    A e;
    string suffix;

    template <typename T>
    void operator()(const T& v) const { os << e(v) << suffix; }
};

template <typename E, typename DX>
auto operator<<(ostream& os, Expr<E, DX> e) {
    return PrintProxy<Expr<E, DX>>{ os, e, "" };
}

template <typename A>
auto operator<<(PrintProxy<A> p, char c) {
    p.suffix += c; return p;
}

template <typename A>
auto operator<<(PrintProxy<A> p, const char* s) {
    p.suffix += s; return p;
}


// ============================================================
// FILTER PROXY  (expr % n == r)
// ============================================================

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
    f.comp = comp; return f;
}
