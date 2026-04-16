using namespace std;

template <int N, typename T>
class Vetor {

private:
    vector<T> principal;

public:
    
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


    Vetor<N, T> operator*(const double b) const {
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
        o << "[ ";
        for(auto x : principal) {
            o << x << " ";
        }
            
        o << "]";
    }
};


template <int N, typename T>
ostream& operator<<(ostream& o, const Vetor<N, T>& v) {
    v.print(o);

    return o;
}


template <int N, typename T>
Vetor<N, T> operator*(double b, const Vetor<N, T>& v) {
    return v * b;
}
