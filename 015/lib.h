#include <memory>

using namespace std;

class Var {
public:

    class Erro {
    public:
        Erro(string msg) : msg(msg) {}
        string operator()() const { return msg; }
    private:
        string msg;
    };

    // Hierarquia de tipos — declarada DENTRO de Var
    class Undefined {
    public:
        virtual ~Undefined() = default;
        virtual void print(ostream& o) const { o << "undefined"; }

        // double dispatch — operação com lado esquerdo de tipo desconhecido
        virtual Var add (const Var&) const { return Var(); }
        virtual Var sub (const Var&) const { return Var(); }
        virtual Var mul (const Var&) const { return Var(); }
        virtual Var div (const Var&) const { return Var(); }
        virtual Var lt  (const Var&) const { return Var(); }
        virtual Var neg ()           const { return Var(); }
        virtual Var land(const Var&) const { return Var(); }
        virtual Var lor (const Var&) const { return Var(); }

        // lado direito com tipo conhecido
        virtual Var addInt   (int    ) const { return Var(); }
        virtual Var addDouble(double ) const { return Var(); }
        virtual Var addString(const string&) const { return Var(); }
        virtual Var subInt   (int    ) const { return Var(); }
        virtual Var subDouble(double ) const { return Var(); }
        virtual Var mulInt   (int    ) const { return Var(); }
        virtual Var mulDouble(double ) const { return Var(); }
        virtual Var divInt   (int    ) const { return Var(); }
        virtual Var divDouble(double ) const { return Var(); }
        virtual Var ltInt    (int    ) const { return Var(); }
        virtual Var ltDouble (double ) const { return Var(); }
        virtual Var ltString (const string&) const { return Var(); }

        // atributos
        virtual Var& attr(const string&) {
            throw Erro("Essa variável não é um objeto");
        }
        virtual const Var& attr(const string&) const {
            throw Erro("Essa variável não suporta acesso a atributos");
        }

        // chamada
        virtual Var call(Var) const {
            throw Erro("Essa variável não pode ser usada como função");
        }

        // bool implícito
        virtual bool toBool() const { return false; }
    };

    // =========================================================
    // Int
    // =========================================================
    class Int : public Undefined {
    public:
        Int(int n) : n(n) {}
        void print(ostream& o) const override { o << n; }
        bool toBool() const override { return n != 0; }

        Var add (const Var& b) const override { return b.valor->addInt(n);    }
        Var sub (const Var& b) const override { return b.valor->subInt(n);    }
        Var mul (const Var& b) const override { return b.valor->mulInt(n);    }
        Var div (const Var& b) const override { return b.valor->divInt(n);    }
        Var lt  (const Var& b) const override { return b.valor->ltInt(n);     }
        Var neg ()             const override { return Var(-n);                }
        Var addInt   (int    x) const override { return Var(x + n);                          }
        Var addDouble(double x) const override { return Var(x + (double)n);                  }
        Var subInt   (int    x) const override { return Var(x - n);                          }
        Var subDouble(double x) const override { return Var(x - (double)n);                  }
        Var mulInt   (int    x) const override { return Var(x * n);                          }
        Var mulDouble(double x) const override { return Var(x * (double)n);                  }
        Var divInt   (int    x) const override { return n == 0 ? Var() : Var(x / n);         }
        Var divDouble(double x) const override { return n == 0 ? Var() : Var(x / (double)n); }
        Var ltInt    (int    x) const override { return Var(x < n);                          }
        Var ltDouble (double x) const override { return Var(x < (double)n);                  }
        Var addString(const string& x) const override {
            if (x.size() == 1) return Var((int)(unsigned char)x[0] + n);
            return Var();
        }
        Var ltString(const string& x) const override {
            if (x.size() == 1) return Var((int)(unsigned char)x[0] < n);
            return Var();
        }
    private:
        int n;
    };

    // =========================================================
    // Double
    // =========================================================
    class Double : public Undefined {
    public:
        Double(double n) : n(n) {}
        void print(ostream& o) const override { o << n; }
        bool toBool() const override { return n != 0.0; }

        Var add (const Var& b)  const override { return b.valor->addDouble(n);   }
        Var sub (const Var& b)  const override { return b.valor->subDouble(n);   }
        Var mul (const Var& b)  const override { return b.valor->mulDouble(n);   }
        Var div (const Var& b)  const override { return b.valor->divDouble(n);   }
        Var lt  (const Var& b)  const override { return b.valor->ltDouble(n);    }
        Var neg ()              const override { return Var(-n);                 }
        Var addInt   (int    x) const override { return Var((double)x + n);      }
        Var addDouble(double x) const override { return Var(x + n);              }
        Var subInt   (int    x) const override { return Var((double)x - n);      }
        Var subDouble(double x) const override { return Var(x - n);              }
        Var mulInt   (int    x) const override { return Var((double)x * n);      }
        Var mulDouble(double x) const override { return Var(x * n);              }
        Var ltDouble (double x) const override { return Var(x < n);              }
        Var ltInt    (int    x) const override { return Var((double)x < n);      }

        Var divInt   (int    x) const override { return n == 0 ? Var() : Var((double)x / n); }
        Var divDouble(double x) const override { return n == 0 ? Var() : Var(x / n);         }
    private:
        double n;
    };

    // String
    class String : public Undefined {
    public:
        String(const string& s) : s(s) {}
        void print(ostream& o) const override { o << s; }
        bool toBool() const override { return !s.empty(); }

        Var add(const Var& b) const override { return b.valor->addString(s); }
        Var lt (const Var& b) const override { return b.valor->ltString(s);  }

        Var addString(const string& x) const override { return Var(x + s); }
        Var ltString (const string& x) const override { return Var(x < s); }

        // String de 1 char comporta-se como seu valor ASCII apenas na SOMA com Int
        // e em comparacoes com Int
        Var addInt(int x) const override {
            if (s.size() == 1) return Var(x + (int)(unsigned char)s[0]);
            return Var();
        }
        Var ltInt(int x) const override {
            if (s.size() == 1) return Var(x < (int)(unsigned char)s[0]);
            return Var();
        }
    private:
        string s;
    };

    // Boolean
    class Boolean : public Undefined {
    public:
        Boolean(bool b) : b(b) {}
        void print(ostream& o) const override { o << (b ? "true" : "false"); }
        bool toBool() const override { return b; }

        Var neg ()             const override { return Var(!b);                             }
        Var land(const Var& r) const override { return Var(b && r.valor->toBool()); }
        Var lor (const Var& r) const override { return Var(b || r.valor->toBool()); }
    private:
        bool b;
    };


    // Object  (map<string, Var>)
    class Object : public Undefined {
    public:
        Object() = default;

        void print(ostream& o) const override {
            o << "object";
        }

        Var& attr(const string& key) override {
            return attrs[key];
        }
        const Var& attr(const string& key) const override {
            static Var undef;
            auto it = attrs.find(key);
            return (it != attrs.end()) ? it->second : undef;
        }
    private:
        map<string, Var> attrs;
    };

    // =========================================================
    // Function  (std::function<Var(Var)>)
    // =========================================================
    class Function : public Undefined {
    public:
        Function(function<Var(Var)> f) : f(move(f)) {}

        void print(ostream& o) const override { o << "function"; }

        Var call(Var arg) const override { return f(arg); }
    private:
        function<Var(Var)> f;
    };

    // =========================================================
    // Construtores de Var
    // =========================================================
    Var()                : valor(make_shared<Undefined>())           {}
    Var(int n)           : valor(make_shared<Int>(n))                {}
    Var(double n)        : valor(make_shared<Double>(n))             {}
    Var(bool b)          : valor(make_shared<Boolean>(b))            {}
    Var(char c)          : valor(make_shared<String>(string(1, c)))  {}
    Var(const char* s)   : valor(make_shared<String>(string(s)))     {}
    Var(const string& s) : valor(make_shared<String>(s))             {}

    // Construtor para callable — template para evitar ambiguidade com bool
    template<typename F,
             typename = typename enable_if<
                 !is_same<typename decay<F>::type, Var>::value &&
                 !is_same<typename decay<F>::type, bool>::value &&
                 !is_integral<typename decay<F>::type>::value &&
                 !is_floating_point<typename decay<F>::type>::value &&
                 !is_same<typename decay<F>::type, string>::value &&
                 !is_same<typename decay<F>::type, const char*>::value>::type>
    Var(F&& f) : valor(make_shared<Function>(function<Var(Var)>(forward<F>(f)))) {}

    // cópia e atribuição — shallow (semântica de referência via shared_ptr)
    Var(const Var&) = default;
    Var& operator=(const Var&) = default;

    // =========================================================
    // Operadores de atribuição para tipos primitivos
    // =========================================================
    Var& operator=(int n)           { valor = make_shared<Int>(n);             return *this; }
    Var& operator=(double n)        { valor = make_shared<Double>(n);          return *this; }
    Var& operator=(bool b)          { valor = make_shared<Boolean>(b);         return *this; }
    Var& operator=(const char* s)   { valor = make_shared<String>(string(s));  return *this; }
    Var& operator=(const string& s) { valor = make_shared<String>(s);          return *this; }

    template<typename F,
             typename = typename enable_if<
                 !is_same<typename decay<F>::type, Var>::value &&
                 !is_same<typename decay<F>::type, bool>::value &&
                 !is_integral<typename decay<F>::type>::value &&
                 !is_floating_point<typename decay<F>::type>::value &&
                 !is_same<typename decay<F>::type, string>::value &&
                 !is_same<typename decay<F>::type, const char*>::value>::type>
    Var& operator=(F&& f) {
        valor = make_shared<Function>(function<Var(Var)>(forward<F>(f)));
        return *this;
    }


    // Factory de Object
    static Var newObject() {
        Var v;
        v.valor = make_shared<Object>();
        return v;
    }


    // Acesso a atributos: a["key"]  e  a->*"key"
    Var& operator[](const string& key)       { return valor->attr(key); }
    const Var& operator[](const string& key) const { return valor->attr(key); }
    Var& operator[](const char* key)         { return valor->attr(string(key)); }

    Var& operator->*(const char* key)        { return valor->attr(string(key)); }
    Var& operator->*(const string& key)      { return valor->attr(key); }


    // Chamada de função: a(b)
    Var operator()(Var arg) const { return valor->call(arg); }


    // Impressão
    void print(ostream& o) const { valor->print(o); }


    // Operadores aritméticos e lógicos
    Var operator+(const Var& b)  const { return valor->add(b);  }
    Var operator-(const Var& b)  const { return valor->sub(b);  }
    Var operator*(const Var& b)  const { return valor->mul(b);  }
    Var operator/(const Var& b)  const { return valor->div(b);  }
    Var operator<(const Var& b)  const { return valor->lt(b);   }
    Var operator!()              const { return valor->neg();   }
    Var operator&&(const Var& b) const { return valor->land(b); }
    Var operator||(const Var& b) const { return valor->lor(b);  }

    // derivados de <
    Var operator> (const Var& b) const { return b < *this;                     }
    Var operator!=(const Var& b) const { return (*this < b) || (b < *this);    }
    Var operator==(const Var& b) const { return !(*this != b);                 }
    Var operator<=(const Var& b) const { return !(b < *this);                  }
    Var operator>=(const Var& b) const { return !(*this < b);                  }

private:
    shared_ptr<Undefined> valor;
};


// operator<< global
inline ostream& operator<<(ostream& o, const Var& v) {
    v.print(o);
    return o;
}


// Operadores globais: primitivo OP Var
// Converte o lado esquerdo em Var e reutiliza os operadores
// de membro — zero duplicação de lógica.
inline Var operator+ (int         a, const Var& b) { return Var(a) + b;  }
inline Var operator- (int         a, const Var& b) { return Var(a) - b;  }
inline Var operator* (int         a, const Var& b) { return Var(a) * b;  }
inline Var operator/ (int         a, const Var& b) { return Var(a) / b;  }
inline Var operator< (int         a, const Var& b) { return Var(a) < b;  }
inline Var operator> (int         a, const Var& b) { return Var(a) > b;  }
inline Var operator<=(int         a, const Var& b) { return Var(a) <= b; }
inline Var operator>=(int         a, const Var& b) { return Var(a) >= b; }
inline Var operator==(int         a, const Var& b) { return Var(a) == b; }
inline Var operator!=(int         a, const Var& b) { return Var(a) != b; }
inline Var operator+ (double      a, const Var& b) { return Var(a) + b;  }
inline Var operator- (double      a, const Var& b) { return Var(a) - b;  }
inline Var operator* (double      a, const Var& b) { return Var(a) * b;  }
inline Var operator/ (double      a, const Var& b) { return Var(a) / b;  }
inline Var operator< (double      a, const Var& b) { return Var(a) < b;  }
inline Var operator> (double      a, const Var& b) { return Var(a) > b;  }
inline Var operator<=(double      a, const Var& b) { return Var(a) <= b; }
inline Var operator>=(double      a, const Var& b) { return Var(a) >= b; }
inline Var operator==(double      a, const Var& b) { return Var(a) == b; }
inline Var operator!=(double      a, const Var& b) { return Var(a) != b; }
inline Var operator+ (const char* a, const Var& b) { return Var(a) + b;  }
inline Var operator< (const char* a, const Var& b) { return Var(a) < b;  }
inline Var operator> (const char* a, const Var& b) { return Var(a) > b;  }
inline Var operator<=(const char* a, const Var& b) { return Var(a) <= b; }
inline Var operator>=(const char* a, const Var& b) { return Var(a) >= b; }
inline Var operator==(const char* a, const Var& b) { return Var(a) == b; }
inline Var operator!=(const char* a, const Var& b) { return Var(a) != b; }


// newObject() livre — confortável no main
inline Var newObject() { return Var::newObject(); }
