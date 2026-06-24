#include <map>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <functional>

using namespace std;

string trim(string s) {
    const string WHITESPACE = " \n\r\t\f\v";
    
    // Find the first non-whitespace character
    size_t start = s.find_first_not_of(WHITESPACE);
    if (start == std::string::npos) {
        return "";
    }
    
    size_t end = s.find_last_not_of(WHITESPACE);
    
    return s.substr(start, end - start + 1);
}

class Var {
public:

    class Erro {
    public:
        Erro(string msg) : msg(msg) {}
        string operator()() const { return msg; }
    private:
        string msg;
    };

    // =========================================================
    // Hierarquia de tipos
    // =========================================================
    class Undefined {
    public:
        virtual ~Undefined() = default;
        virtual void print(ostream& o) const { o << "undefined"; }

        virtual Var add (const Var&) const { return Var(); }
        virtual Var sub (const Var&) const { return Var(); }
        virtual Var mul (const Var&) const { return Var(); }
        virtual Var div (const Var&) const { return Var(); }
        virtual Var lt  (const Var&) const { return Var(); }
        virtual Var neg ()           const { return Var(true); }
        virtual Var land(const Var&) const { return Var(); }
        virtual Var lor (const Var&) const { return Var(); }
        virtual Var mod (const Var&) const { return Var(); }

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
        virtual Var modInt   (int    ) const { return Var(); }
        virtual Var modDouble(double ) const { return Var(); }

        virtual Var& attr(const string&) {
            throw Erro("Essa variável não é um objeto");
        }
        virtual const Var& attr(const string&) const {
            throw Erro("Essa variável não suporta acesso a atributos");
        }

        // Indexação inteira — só Arrays sobrescrevem
        virtual Var& attrInt(int i) {
            return attr(to_string(i));
        }
        virtual const Var& attrInt(int i) const {
            return attr(to_string(i));
        }

        virtual Var call(Var) const {
            throw Erro("Essa variável não pode ser usada como função");
        }

        // conversões
        virtual bool   toBool()   const { return false; }
        virtual double toDouble() const { return 0.0; }
        virtual string toString() const { return "undefined"; }

        // classificadores
        virtual bool kindIsNumber()    const { return false; }
        virtual bool kindIsString()    const { return false; }
        virtual bool kindIsBool()      const { return false; }
        virtual bool kindIsObject()    const { return false; }
        virtual bool kindIsUndefined() const { return true;  }
        virtual bool kindIsArray()     const { return false; }
        virtual bool kindIsNumeric()   const { return false; }
    };

    // =========================================================
    // Int
    // =========================================================
    class Int : public Undefined {
    public:
        Int(int n) : n(n) {}
        void print(ostream& o) const override { o << n; }

        bool   toBool()   const override { return n != 0; }
        double toDouble() const override { return (double)n; }
        string toString() const override { return to_string(n); }

        bool kindIsNumber()    const override { return true;  }
        bool kindIsUndefined() const override { return false; }

        Var add (const Var& b) const override { return b.valor->addInt(n);    }
        Var sub (const Var& b) const override { return b.valor->subInt(n);    }
        Var mul (const Var& b) const override { return b.valor->mulInt(n);    }
        Var div (const Var& b) const override { return b.valor->divInt(n);    }
        Var lt  (const Var& b) const override { return b.valor->ltInt(n);     }
        Var neg ()             const override { return n == 0 ? Var(true) : Var(false); }
        Var mod(const Var& b) const override { return b.valor->modInt(n); }

        Var addInt   (int    x) const override { return Var(x + n);                           }
        Var addDouble(double x) const override { return Var(x + (double)n);                   }
        Var subInt   (int    x) const override { return Var(x - n);                           }
        Var subDouble(double x) const override { return Var(x - (double)n);                   }
        Var mulInt   (int    x) const override { return Var(x * n);                           }
        Var mulDouble(double x) const override { return Var(x * (double)n);                   }
        Var divInt   (int    x) const override { return n == 0 ? Var() : Var(x / n);          }
        Var divDouble(double x) const override { return n == 0 ? Var() : Var(x / (double)n);  }
        Var ltInt    (int    x) const override { return Var(x < n);                           }
        Var ltDouble (double x) const override { return Var(x < (double)n);                   }
        
        Var modInt   (int    x) const override { return n == 0 ? Var() : Var(x % n);                    }
        Var modDouble(double x) const override { return n == 0 ? Var() : Var(fmod(x, (double)n)); }
        
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

        bool   toBool()   const override { return n != 0.0; }
        double toDouble() const override { return n; }
        string toString() const override {
            ostringstream oss; oss << n; return oss.str();
        }

        bool kindIsNumber()    const override { return true;  }
        bool kindIsUndefined() const override { return false; }

        Var add (const Var& b)  const override { return b.valor->addDouble(n);   }
        Var sub (const Var& b)  const override { return b.valor->subDouble(n);   }
        Var mul (const Var& b)  const override { return b.valor->mulDouble(n);   }
        Var div (const Var& b)  const override { return b.valor->divDouble(n);   }
        Var lt  (const Var& b)  const override { return b.valor->ltDouble(n);    }
        Var neg ()              const override { return Var(-n);                 }
        Var mod(const Var& b)   const override { return b.valor->modDouble(n);   }

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

        Var modInt   (int    x) const override { return n == 0.0 ? Var() : Var(fmod((double)x, n)); }
        Var modDouble(double x) const override { return n == 0.0 ? Var() : Var(fmod(x, n));         }
    private:
        double n;
    };

    // =========================================================
    // String
    // =========================================================
    class String : public Undefined {
    public:
        String(const string& s) : s(s) {}
        void print(ostream& o) const override { o << s; }

        // JS: "" e "0" são falsos, qualquer outra string é verdadeira
        bool   toBool()   const override { return !s.empty(); }
        double toDouble() const override {
            try { return stod(s); } catch(...) { return 0.0; }
        }
        string toString() const override { return s; }

        bool kindIsNumeric() const override {
            string trimmed = trim(s);

            if (trimmed.empty())        // string vazia ou só whitespace → numérica (vale 0)
                return true;

            size_t pos;
            try {
                double d = stod(trimmed, &pos);
                return pos == trimmed.size() && !isnan(d) && !isinf(d);
            }
            catch (...) { return false; }
        }

        bool kindIsString()    const override { return true;  }
        bool kindIsUndefined() const override { return false; }

        Var add(const Var& b) const override { return b.valor->addString(s); }
        Var lt (const Var& b) const override { return b.valor->ltString(s);  }

        Var neg ()            const override { return s.empty() ? Var(true) : Var(false); }

        Var addString(const string& x) const override { return Var(x + s); }
        Var ltString (const string& x) const override { return Var(x < s); }

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

    // =========================================================
    // Boolean
    // =========================================================
    class Boolean : public Undefined {
    public:
        Boolean(bool b) : b(b) {}
        void print(ostream& o) const override { o << (b ? "true" : "false"); }

        bool   toBool()   const override { return b; }
        double toDouble() const override { return b ? 1.0 : 0.0; }
        string toString() const override { return b ? "true" : "false"; }

        bool kindIsNumber()    const override { return true;  }
        bool kindIsBool()      const override { return true;  }
        bool kindIsUndefined() const override { return false; }

        Var neg ()             const override { return Var(!b);                     }
        Var land(const Var& r) const override { return Var(b && r.valor->toBool()); }
        Var lor (const Var& r) const override { return Var(b || r.valor->toBool()); }
    private:
        bool b;
    };

    // =========================================================
    // Object
    // =========================================================
    class Object : public Undefined {
    public:
        Object() = default;

        void print(ostream& o) const override { o << "object"; }

        bool   toBool()   const override { return true; }
        double toDouble() const override { return 0.0; }
        string toString() const override { return "object"; }

        bool kindIsObject()    const override { return true;  }
        bool kindIsUndefined() const override { return false; }

        Var neg () const override { return attrs.size() == 0 ? Var(false) : Var(true); }

        Var& attr(const string& key) override { return attrs[key]; }
        const Var& attr(const string& key) const override {
            static Var undef;
            auto it = attrs.find(key);
            return (it != attrs.end()) ? it->second : undef;
        }

    protected:
        map<string, Var> attrs;
    };

    // =========================================================
    // Array  (subclasse de Object)
    // =========================================================
    class Array : public Object {
    public:
        Array() {
            // instala os quatro métodos funcionais como propriedades
            _installMethods();
        }

        void print(ostream& o) const override {
            o << "[";
            for (size_t i = 0; i < elems.size(); ++i) {
                if (i) o << ",";
                elems[i].print(o);
            }
            o << "]";
        }

        bool   toBool()   const override { return true; }
        string toString() const override { return "array"; }

        bool kindIsArray()  const override { return true; }
        bool kindIsObject() const override { return true; }

        // Índice inteiro não-negativo → elemento do vetor
        // Índice negativo → propriedade string (como JS)
        Var& attrInt(int i) override {
            if (i < 0) return attr(to_string(i));
            auto idx = (size_t)i;
            if (idx >= elems.size()) elems.resize(idx + 1);
            return elems[idx];
        }
        const Var& attrInt(int i) const override {
            if (i < 0) return attr(to_string(i));
            auto idx = (size_t)i;
            static Var undef;
            if (idx >= elems.size()) return undef;
            return elems[idx];
        }

        // Acesso por string: "0","1",... → elemento; outros → propriedade herdada
        Var& attr(const string& key) override {
            // tenta parsear como índice inteiro não-negativo
            bool allDigits = !key.empty();
            for (char c : key) if (!isdigit((unsigned char)c)) { allDigits = false; break; }
            if (allDigits) {
                int idx = stoi(key);
                if ((size_t)idx >= elems.size()) elems.resize((size_t)idx + 1);
                return elems[(size_t)idx];
            }
            return Object::attr(key);
        }
        const Var& attr(const string& key) const override {
            bool allDigits = !key.empty();
            for (char c : key) if (!isdigit((unsigned char)c)) { allDigits = false; break; }
            if (allDigits) {
                int idx = stoi(key);
                static Var undef;
                if ((size_t)idx >= elems.size()) return undef;
                return elems[(size_t)idx];
            }
            return Object::attr(key);
        }

        size_t size() const { return elems.size(); }
        vector<Var>& elements() { return elems; }
        const vector<Var>& elements() const { return elems; }

    private:
        vector<Var> elems;

        void _installMethods();   // definido após Var estar completo
    };

    // =========================================================
    // Function
    // =========================================================
    class Function : public Undefined {
    public:
        template<typename F>
        Function(F&& fn) {
            if constexpr (is_invocable_r_v<Var, F, Var>) {
                f = function<Var(Var)>(forward<F>(fn));
            } else if constexpr (is_invocable_v<F, int>) {
                if constexpr (is_void_v<invoke_result_t<F, int>>) {
                    f = [fn = forward<F>(fn)](Var v) -> Var { fn((int)v.asNumber()); return Var(); };
                } else {
                    f = [fn = forward<F>(fn)](Var v) -> Var { return fn((int)v.asNumber()); };
                }
            } else if constexpr (is_invocable_v<F, double>) {
                if constexpr (is_void_v<invoke_result_t<F, double>>) {
                    f = [fn = forward<F>(fn)](Var v) -> Var { fn(v.asNumber()); return Var(); };
                } else {
                    f = [fn = forward<F>(fn)](Var v) -> Var { return fn(v.asNumber()); };
                }
            } else {
                if constexpr (is_void_v<invoke_result_t<F, Var>>) {
                    f = [fn = forward<F>(fn)](Var v) -> Var { fn(v); return Var(); };
                } else {
                    f = [fn = forward<F>(fn)](Var v) -> Var { return fn(v); };
                }
            }
        }

        void print(ostream& o) const override { o << "function"; }
        bool kindIsUndefined() const override { return false; }
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

    template<typename F,
             typename = typename enable_if<
                 !is_same<typename decay<F>::type, Var>::value &&
                 !is_same<typename decay<F>::type, bool>::value &&
                 !is_integral<typename decay<F>::type>::value &&
                 !is_floating_point<typename decay<F>::type>::value &&
                 !is_same<typename decay<F>::type, string>::value &&
                 !is_same<typename decay<F>::type, const char*>::value>::type>
    Var(F&& f) : valor(make_shared<Function>(forward<F>(f))) {}

    Var(const Var&) = default;
    Var& operator=(const Var&) = default;

    // =========================================================
    // Operadores de atribuição para tipos primitivos
    // =========================================================
    Var& operator=(int n)           { valor = make_shared<Int>(n);             return *this; }
    Var& operator=(double n)        { valor = make_shared<Double>(n);          return *this; }
    Var& operator=(bool b)          { valor = make_shared<Boolean>(b);         return *this; }
    Var& operator=(char c)          { valor = make_shared<String>(string(1,c)); return *this; }
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
        valor = make_shared<Function>(forward<F>(f));
        return *this;
    }

    // =========================================================
    // Factories
    // =========================================================
    static Var newObject() {
        Var v;
        v.valor = make_shared<Object>();
        return v;
    }
    static Var newArray() {
        Var v;
        v.valor = make_shared<Array>();
        return v;
    }

    // =========================================================
    // Conversões de tipo (comportamento JS)
    // =========================================================
    bool   asBool()   const { return valor->toBool();   }
    double asNumber() const { return valor->toDouble(); }
    string asString() const { return valor->toString(); }

    // =========================================================
    // Classificadores de tipo
    // =========================================================
    bool isNumber()    const { return valor->kindIsNumber() || valor->kindIsNumeric();    }
    bool isString()    const { return valor->kindIsString();    }
    bool isBool()      const { return valor->kindIsBool();      }
    bool isObject()    const { return valor->kindIsObject();    }
    bool isUndefined() const { return valor->kindIsUndefined(); }
    bool isArray()     const { return valor->kindIsArray();     }

    // =========================================================
    // Acesso a atributos por string
    // =========================================================
    Var& operator[](const string& key)         { return valor->attr(key); }
    const Var& operator[](const string& key) const { return valor->attr(key); }
    Var& operator[](const char* key)           { return valor->attr(string(key)); }
    const Var& operator[](const char* key) const { return valor->attr(string(key)); }

    // Acesso por inteiro — delega a attrInt para que Array trate índices negativos
    Var& operator[](int i) {
        if (i >= 0) return valor->attrInt(i);
        return valor->attr(to_string(i));
    }
    const Var& operator[](int i) const {
        if (i >= 0) return valor->attrInt(i);
        return valor->attr(to_string(i));
    }

    // Acesso por Var (converte para int ou string conforme o tipo)
    Var& operator[](const Var& key) {
        if (key.isNumber()) return (*this)[(int)key.asNumber()];
        return valor->attr(key.asString());
    }
    const Var& operator[](const Var& key) const {
        if (key.isNumber()) return (*this)[(int)key.asNumber()];
        return valor->attr(key.asString());
    }

    Var& operator->*(const char* key)   { return valor->attr(string(key)); }
    Var& operator->*(const string& key) { return valor->attr(key); }

    // =========================================================
    // Chamada de função
    // =========================================================
    Var operator()(Var arg) const { return valor->call(arg); }

    // =========================================================
    // Impressão
    // =========================================================
    void print(ostream& o) const { valor->print(o); }

    // =========================================================
    // Operadores aritméticos e lógicos
    // =========================================================
    Var operator+(const Var& b)  const { return valor->add(b);  }
    Var operator-(const Var& b)  const { return valor->sub(b);  }
    Var operator*(const Var& b)  const { return valor->mul(b);  }
    Var operator/(const Var& b)  const { return valor->div(b);  }
    Var operator<(const Var& b)  const { return valor->lt(b);   }
    Var operator!()              const { return valor->neg();   }
    Var operator&&(const Var& b) const { return valor->land(b); }
    Var operator||(const Var& b) const { return valor->lor(b);  }
    Var operator% (const Var& b) const { return valor->mod(b);  }

    Var operator> (const Var& b) const { return b < *this;                   }
    Var operator!=(const Var& b) const {
        Var l = *this < b;
        Var r = b < *this;
        
        // se ambos undefined, tipos incomparáveis → são diferentes
        if (l.isUndefined() && r.isUndefined()) return Var(true);
        
        return l || r;
    }
    Var operator==(const Var& b) const { return !(*this != b);               }
    Var operator<=(const Var& b) const { return !(b < *this);                }
    Var operator>=(const Var& b) const { return !(*this < b);                }

    // =========================================================
    // Acesso interno ao ponteiro (necessário para métodos de Array)
    // =========================================================
    shared_ptr<Undefined>& _valor() { return valor; }
    const shared_ptr<Undefined>& _valor() const { return valor; }


    Var forEach(Var fn) const { return (*this)["forEach"](fn); }
    Var filter (Var fn) const { return (*this)["filter"](fn);  }
    Var map    (Var fn) const { return (*this)["map"](fn);     }
    Var indexOf(Var v)  const { return (*this)["indexOf"](v);  }

private:
    shared_ptr<Undefined> valor;
};

// =========================================================
// Array::_installMethods  — definido aqui porque usa Var completo
// =========================================================
inline void Var::Array::_installMethods() {

    // ----- indexOf -----
    attrs["indexOf"] = Var([this](Var target) -> Var {
        for (size_t i = 0; i < elems.size(); ++i)
            if ((elems[i] == target).asBool()) return Var((int)i);
        return Var(-1);
    });

    // ----- forEach -----
    attrs["forEach"] = Var([this](Var fn) -> Var {
        for (size_t i = 0; i < elems.size(); ++i)
            fn(elems[i]);
        return Var();
    });

    // ----- filter -----
    attrs["filter"] = Var([this](Var pred) -> Var {
        Var result = Var::newArray();
        auto* arr = static_cast<Array*>(result._valor().get());
        for (size_t i = 0; i < elems.size(); ++i)
            if (pred(elems[i]).asBool())
                arr->elements().push_back(elems[i]);
        // reinstala métodos no novo array após preenchimento
        arr->_installMethods();
        return result;
    });

    // ----- map -----
    attrs["map"] = Var([this](Var fn) -> Var {
        Var result = Var::newArray();
        auto* arr = static_cast<Array*>(result._valor().get());
        arr->elements().resize(elems.size());
        for (size_t i = 0; i < elems.size(); ++i)
            arr->elements()[i] = fn(elems[i]);
        arr->_installMethods();
        return result;
    });
}

// =========================================================
// operator<< global
// =========================================================
inline ostream& operator<<(ostream& o, const Var& v) {
    v.print(o);
    return o;
}

// =========================================================
// newArray / newObject livres
// =========================================================
inline Var newObject() { return Var::newObject(); }
inline Var newArray()  { return Var::newArray();  }

// =========================================================
// Operadores globais: primitivo OP Var
// =========================================================
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
inline Var operator% (int         a, const Var& b) { return Var(a) % b;  }
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
inline Var operator% (double      a, const Var& b) { return Var(a) % b;  }
inline Var operator+ (const char* a, const Var& b) { return Var(a) + b;  }
inline Var operator< (const char* a, const Var& b) { return Var(a) < b;  }
inline Var operator> (const char* a, const Var& b) { return Var(a) > b;  }
inline Var operator<=(const char* a, const Var& b) { return Var(a) <= b; }
inline Var operator>=(const char* a, const Var& b) { return Var(a) >= b; }
inline Var operator==(const char* a, const Var& b) { return Var(a) == b; }
inline Var operator!=(const char* a, const Var& b) { return Var(a) != b; }
