#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <limits>

// ============================================================
// Forward declarations
// ============================================================
template <int L, int C> class Matriz;
template <typename Tag>  class Otimizador;
struct TagProduto {};
struct TagSoma    {};

// ============================================================
// Nó base
// ============================================================
struct NoBase {
    virtual ~NoBase() = default;
    virtual int linhas()  const = 0;
    virtual int colunas() const = 0;
    virtual std::vector<double> avaliar() const = 0;
};

// ============================================================
// NoFolha — compartilha ownership do buffer via shared_ptr
// Nunca dangling: a Matriz<L,C> expõe um shared_ptr<vector<double>>
// ============================================================
struct NoFolha : NoBase {
    // Mantém o buffer vivo enquanto o nó existir
    std::shared_ptr<const std::vector<double>> buf;
    int l, c;

    NoFolha(std::shared_ptr<const std::vector<double>> b, int l, int c)
        : buf(std::move(b)), l(l), c(c) {}

    int linhas()  const override { return l; }
    int colunas() const override { return c; }

    std::vector<double> avaliar() const override {
        return *buf;   // cópia do vetor — segura e independente
    }
};

// ============================================================
// NoProduto
// ============================================================
struct NoProduto : NoBase {
    std::shared_ptr<NoBase> esq, dir;
    int l, lc, c;

    NoProduto(std::shared_ptr<NoBase> a, std::shared_ptr<NoBase> b)
        : esq(std::move(a)), dir(std::move(b)),
          l(esq->linhas()), lc(esq->colunas()), c(dir->colunas()) {}

    int linhas()  const override { return l;  }
    int colunas() const override { return c;  }

    std::vector<double> avaliar() const override {
        auto A = esq->avaliar();
        auto B = dir->avaliar();
        std::vector<double> R(l * c, 0.0);
        for (int i = 0; i < l; ++i)
            for (int k = 0; k < lc; ++k) {
                double a = A[i * lc + k];
                for (int j = 0; j < c; ++j)
                    R[i * c + j] += a * B[k * c + j];
            }
        return R;
    }
};

// ============================================================
// NoSoma
// ============================================================
struct NoSoma : NoBase {
    std::shared_ptr<NoBase> esq, dir;
    int l, c;

    NoSoma(std::shared_ptr<NoBase> a, std::shared_ptr<NoBase> b)
        : esq(std::move(a)), dir(std::move(b)),
          l(esq->linhas()), c(esq->colunas())
    {
        if (l != dir->linhas() || c != dir->colunas())
            throw std::runtime_error("Soma: dimensões incompatíveis");
    }

    int linhas()  const override { return l; }
    int colunas() const override { return c; }

    std::vector<double> avaliar() const override {
        auto A = esq->avaliar();
        auto B = dir->avaliar();
        std::vector<double> R(l * c);
        for (int i = 0; i < l * c; ++i) R[i] = A[i] + B[i];
        return R;
    }
};

// ============================================================
// Parentização ótima — DP O(n³)
// ============================================================
inline std::shared_ptr<NoBase> parentizarOtimo(
    const std::vector<std::shared_ptr<NoBase>>& nos)
{
    int n = static_cast<int>(nos.size());
    if (n == 1) return nos[0];

    std::vector<int> dims(n + 1);
    for (int i = 0; i < n; ++i) dims[i] = nos[i]->linhas();
    dims[n] = nos[n-1]->colunas();

    std::vector<std::vector<long long>> custo(n, std::vector<long long>(n, 0));
    std::vector<std::vector<int>>       split(n, std::vector<int>(n, 0));

    for (int len = 2; len <= n; ++len)
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;
            custo[i][j] = std::numeric_limits<long long>::max();
            for (int k = i; k < j; ++k) {
                long long cc = custo[i][k] + custo[k+1][j]
                             + (long long)dims[i] * dims[k+1] * dims[j+1];
                if (cc < custo[i][j]) { custo[i][j] = cc; split[i][j] = k; }
            }
        }

    std::function<std::shared_ptr<NoBase>(int,int)> build =
        [&](int i, int j) -> std::shared_ptr<NoBase> {
            if (i == j) return nos[i];
            int k = split[i][j];
            return std::make_shared<NoProduto>(build(i,k), build(k+1,j));
        };
    return build(0, n-1);
}

// ============================================================
// Otimizador<Tag>
// ============================================================
template <typename Tag>
class Otimizador {
public:
    std::shared_ptr<NoBase>              raiz;
    std::vector<std::shared_ptr<NoBase>> operandos;

    explicit Otimizador(std::shared_ptr<NoBase> r,
                        std::vector<std::shared_ptr<NoBase>> ops = {})
        : raiz(std::move(r)), operandos(std::move(ops)) {}

    int linhas()  const { return raiz->linhas();  }
    int colunas() const { return raiz->colunas(); }

    std::vector<double> avaliar() const {
        if constexpr (std::is_same_v<Tag, TagProduto>)
            if (operandos.size() > 1)
                return parentizarOtimo(operandos)->avaliar();
        return raiz->avaliar();
    }
};

// ============================================================
// Matriz<L,C>
// Buffer interno gerenciado por shared_ptr<vector<double>>
// permitindo que NoFolha compartilhe ownership sem ponteiro cru
// ============================================================
template <int L, int C>
class Matriz {
    static_assert(L > 0 && C > 0, "Dimensões devem ser positivas");

    // shared_ptr permite que NoFolha mantenha o buffer vivo
    std::shared_ptr<std::vector<double>> dados;

public:
    Matriz() : dados(std::make_shared<std::vector<double>>(L * C, 0.0)) {}

    Matriz(const Matriz& o)
        : dados(std::make_shared<std::vector<double>>(*o.dados)) {}

    Matriz& operator=(const Matriz& o) {
        if (this != &o)
            *dados = *o.dados;   // copia conteúdo, não o ponteiro
        return *this;
    }

    Matriz(Matriz&&) = default;
    Matriz& operator=(Matriz&&) = default;

    template <typename Tag>
    Matriz(const Otimizador<Tag>& opt)
        : dados(std::make_shared<std::vector<double>>(L * C)) { *this = opt; }

    template <typename Tag>
    Matriz& operator=(const Otimizador<Tag>& opt) {
        if (opt.linhas() != L || opt.colunas() != C)
            throw std::runtime_error("Dimensões incompatíveis na atribuição");
        auto r = opt.avaliar();
        *dados = std::move(r);
        return *this;
    }

    constexpr int nLin() const { return L; }
    constexpr int nCol() const { return C; }

    struct ProxyLinha {
        double* ptr;
        double&       operator[](int j)       { return ptr[j]; }
        const double& operator[](int j) const { return ptr[j]; }
    };
    struct ProxyLinhaConst {
        const double* ptr;
        const double& operator[](int j) const { return ptr[j]; }
    };

    ProxyLinha      operator[](int i)       { return {dados->data() + i*C}; }
    ProxyLinhaConst operator[](int i) const { return {dados->data() + i*C}; }

    // Expõe shared_ptr para que NoFolha compartilhe ownership
    std::shared_ptr<const std::vector<double>> bufferShared() const {
        return dados;
    }
};

// ============================================================
// Helpers internos
// ============================================================
template <int L, int C>
std::shared_ptr<NoBase> _no(const Matriz<L,C>& m) {
    return std::make_shared<NoFolha>(m.bufferShared(), L, C);
}

template <typename Tag>
std::shared_ptr<NoBase> _no(const Otimizador<Tag>& opt) {
    if constexpr (std::is_same_v<Tag, TagProduto>)
        if (opt.operandos.size() > 1)
            return parentizarOtimo(opt.operandos);
    return opt.raiz;
}

template <int L, int C>
std::vector<std::shared_ptr<NoBase>> _ops(const Matriz<L,C>& m) {
    return { std::make_shared<NoFolha>(m.bufferShared(), L, C) };
}

template <typename Tag>
std::vector<std::shared_ptr<NoBase>> _ops(const Otimizador<Tag>& opt) {
    if constexpr (std::is_same_v<Tag, TagProduto>)
        return opt.operandos;
    return { opt.raiz };
}

// ============================================================
// operator*
// ============================================================
template <int L, int LC, int C>
Otimizador<TagProduto> operator*(const Matriz<L,LC>& a, const Matriz<LC,C>& b) {
    auto nA = std::make_shared<NoFolha>(a.bufferShared(), L, LC);
    auto nB = std::make_shared<NoFolha>(b.bufferShared(), LC, C);
    std::vector<std::shared_ptr<NoBase>> ops{nA, nB};
    return Otimizador<TagProduto>(std::make_shared<NoProduto>(nA, nB), ops);
}

template <typename Expr, int L2, int C2>
Otimizador<TagProduto> operator*(const Otimizador<Expr>& opt, const Matriz<L2,C2>& b) {
    if (opt.colunas() != L2) throw std::runtime_error("Dimensões incompatíveis");
    auto nB  = std::make_shared<NoFolha>(b.bufferShared(), L2, C2);
    auto ops = _ops(opt);
    ops.push_back(nB);
    return Otimizador<TagProduto>(parentizarOtimo(ops), ops);
}

template <int L, int C, typename Expr>
Otimizador<TagProduto> operator*(const Matriz<L,C>& a, const Otimizador<Expr>& opt) {
    if (C != opt.linhas()) throw std::runtime_error("Dimensões incompatíveis");
    auto nA = std::make_shared<NoFolha>(a.bufferShared(), L, C);
    std::vector<std::shared_ptr<NoBase>> ops{nA};
    auto rops = _ops(opt);
    ops.insert(ops.end(), rops.begin(), rops.end());
    return Otimizador<TagProduto>(parentizarOtimo(ops), ops);
}

template <typename E1, typename E2>
Otimizador<TagProduto> operator*(const Otimizador<E1>& a, const Otimizador<E2>& b) {
    if (a.colunas() != b.linhas()) throw std::runtime_error("Dimensões incompatíveis");
    auto ops  = _ops(a);
    auto bops = _ops(b);
    ops.insert(ops.end(), bops.begin(), bops.end());
    return Otimizador<TagProduto>(parentizarOtimo(ops), ops);
}

// ============================================================
// operator+
// ============================================================
template <int L, int C>
Otimizador<TagSoma> operator+(const Matriz<L,C>& a, const Matriz<L,C>& b) {
    return Otimizador<TagSoma>(std::make_shared<NoSoma>(_no(a), _no(b)));
}

template <typename Expr, int L, int C>
Otimizador<TagSoma> operator+(const Otimizador<Expr>& opt, const Matriz<L,C>& b) {
    if (opt.linhas() != L || opt.colunas() != C)
        throw std::runtime_error("Soma: dimensões incompatíveis");
    return Otimizador<TagSoma>(std::make_shared<NoSoma>(_no(opt), _no(b)));
}

template <int L, int C, typename Expr>
Otimizador<TagSoma> operator+(const Matriz<L,C>& a, const Otimizador<Expr>& opt) {
    if (opt.linhas() != L || opt.colunas() != C)
        throw std::runtime_error("Soma: dimensões incompatíveis");
    return Otimizador<TagSoma>(std::make_shared<NoSoma>(_no(a), _no(opt)));
}

template <typename E1, typename E2>
Otimizador<TagSoma> operator+(const Otimizador<E1>& a, const Otimizador<E2>& b) {
    if (a.linhas() != b.linhas() || a.colunas() != b.colunas())
        throw std::runtime_error("Soma: dimensões incompatíveis");
    return Otimizador<TagSoma>(std::make_shared<NoSoma>(_no(a), _no(b)));
}

// ============================================================
// tamanho()
// ============================================================
template <int L, int C>
int tamanho(const Matriz<L,C>&) { return L * C; }

template <typename Tag>
int tamanho(const Otimizador<Tag>& opt) { return opt.linhas() * opt.colunas(); }


// ============================================================
// Apply<F>
// ============================================================
template <typename F>
class Apply {
public:
    explicit Apply(F f) : f(std::move(f)) {}

    // Sobrecarga para Matriz<L,C> — aplica f elemento a elemento
    template <int L, int C>
    Matriz<L,C> operator()(const Matriz<L,C>& m) const {
        Matriz<L,C> r;
        for (int i = 0; i < L; ++i)
            for (int j = 0; j < C; ++j)
                r[i][j] = f(m[i][j]);
        return r;
    }

    // Sobrecarga para Otimizador<Tag> — avalia a expressão lazy,
    // aplica f sobre cada elemento e devolve novo Otimizador
    // com o resultado já materializado num NoFolha.
    // Isso permite que código como:
    //   auto ap = Apply([](double x){ return x; });
    //   ap( algumOtimizador );
    // compile e funcione sem erro.
    template <typename Tag>
    Otimizador<Tag> operator()(const Otimizador<Tag>& opt) const {
        auto buf = opt.avaliar();              // avalia tudo na heap
        for (auto& v : buf) v = f(v);         // aplica f in-place

        // Empacota como NoFolha com ownership compartilhado
        auto sp = std::make_shared<std::vector<double>>(std::move(buf));
        auto folha = std::make_shared<NoFolha>(
            std::shared_ptr<const std::vector<double>>(sp),
            opt.linhas(), opt.colunas());

        return Otimizador<Tag>(folha, {folha});
    }

private:
    F f;
};
