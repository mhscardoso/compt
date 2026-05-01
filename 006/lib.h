using namespace std;

#include <vector>

template <int N, typename T>
class Vetor {

private:
    vector<T> principal;

public:

    class ProdVet {

    private:
        const Vetor<N, T>& a;

        ProdVet(const Vetor<N, T>& primeiro): a(primeiro) {}
        friend class Vetor<N, T>;

    public:
        
        Vetor<N, T> vetorial(const Vetor<N, T>& b) const {
            Vetor<N, T> resultado;

            resultado.insere(b.get(1)*a.get(2) - b.get(2)*a.get(1));
            resultado.insere(b.get(2)*a.get(0) - b.get(0)*a.get(2));
            resultado.insere(b.get(0)*a.get(1) - b.get(1)*a.get(0));

            return resultado;
        }
    };


    ProdVet operator*() const {
        return ProdVet(*this);
    }

    
    Vetor& operator=(const initializer_list<T>& lista) {
        if (lista.size() != N) {
            exit(1);
        }

        principal.clear();

        for (const auto& e : lista) {
            principal.push_back(e);
        }

        return *this;
    }


    void insere(const T e) {
        if (principal.size() >= N) {
            return;
        }

        principal.push_back(e);
    }


    T get(int n) const {
        return principal.at(n);
    }


    Vetor<N, T> operator+(const Vetor<N, T>& b) const {
        Vetor<N, T> resultado;

        for (int i = 0; i < N; ++i) {
            T soma = this->get(i) + b.get(i);
            resultado.insere(soma);
        }

        return resultado;
    }


    Vetor<N, T> operator*(const T b) const {
        Vetor<N, T> resultado;

        for (int i = 0; i < N; ++i) {
            T soma = this->get(i) * b;
            resultado.insere(soma);
        }

        return resultado;
    }


    T operator*(const Vetor<N, T>& b) const {
        Vetor<N, T> resultado;

        T soma = 0;

        for (int i = 0; i < N; ++i) {
            soma += this->get(i) * b.get(i);
            resultado.insere(soma);
        }

        return soma;
    }


    void print(ostream& o) const {
        for(auto x = principal.begin(); x != principal.end(); ++x) {
            o << *x;

            if (x < principal.end() - 1) {
                o << " ";
            }
        }
    }
};


template <int N, typename T>
ostream& operator<<(ostream& o, const Vetor<N, T>& v) {
    v.print(o);

    return o;
}


template <int N, typename T>
Vetor<N, T> operator*(T b, const Vetor<N, T>& v) {
    return v * b;
}


template <int N, typename T>
Vetor<N, T> operator*(const Vetor<N, T>& v, const typename Vetor<N, T>::ProdVet& p) {
    return p.vetorial(v);
}
