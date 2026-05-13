using namespace std;

class Global {
public:
	double v(double x) const {
		return x;
	}

	double dx(double) const {
		return 1;
	}
};

Global x;

class Const {
private:
    double c;

public:
    Const(double c) : c(c) {}

    double v(double) const {
        return c;
    }

    double dx(double) const {
        return 0;
    }
};


class Expr {
public:
	virtual ~Expr() = default;
	virtual double v(double x) = 0;
	virtual double dx(double x) = 0;
};


template <typename L, typename R>
class Mul : public Expr {
private:
	L l;
    R r;

public:
    Mul(L l, R r) : l(l), r(r) {}

    double v(double x) const override {
        return l.v(x) * r.v(x);
    }

    double dx(double x) const override {
        // regra do produto
        return l.dx(x) * r.v(x) + l.v(x)  * r.dx(x);
    }
};


template <typename L, typename R>
class Add : public Expr {
private:
	L l;
    R r;

public:
    Add(L l, R r) : l(l), r(r) {}

    double v(double x) const override {
        return l.v(x) + r.v(x);
    }

    double dx(double x) const override {
        return l.dx(x) + r.dx(x);
    }
};


template <typename L, typename R>
class Sub : public Expr {
private:
	L l;
    R r;

public:
    Sub(L l, R r) : l(l), r(r) {}

    double v(double x) const override {
        return l.v(x) - r.v(x);
    }

    double dx(double x) const override {
        return l.dx(x) - r.dx(x);
    }
};


template <typename L, typename R>
class Div : public Expr {
private:
	L l;
    R r;

public:
    Div(L l, R r) : l(l), r(r) {}

    double v(double x) const override {
        return l.v(x) / r.v(x);
    }

    double dx(double x) const override {
        return (l.dx(x) * r.v(x) + l.v(x) * r.dx(x)) / (r.v(x) * r.v(x));
    }
};


template <typename T>
auto make_expr(T v) {
    if constexpr (is_arithmetic_v<T>)
        return Const(v);
    else
        return v;
}


template <typename L, typename R>
auto operator*(L l, R r) {
	return Mul<decltype(make_expr(l)), decltype(make_expr(r))>(make_expr(l), make_expr(r));
}


// template <typename R>
// auto operator*(double c, R r) {
//     return Mul<Const, R>(Const(c), r);
// }


// template <typename R>
// auto operator*(int c, R r) {
//     return Mul<Const, R>(Const(c), r);
// }


// template <typename L>
// auto operator*(L l, double c) {
//     return Mul<L, Const>(l, Const(c));
// }

// template <typename L>
// auto operator*(L l, int c) {
//     return Mul<L, Const>(l, Const(c));
// }


// template <typename L, typename R>
// Add<L, R> operator+(L l, R r) {
//     return Add<L, R>(l, r);
// }


// template <typename R>
// auto operator+(double c, R r) {
//     return Add<Const, R>(Const(c), r);
// }


// template <typename R>
// auto operator+(int c, R r) {
//     return Add<Const, R>(Const(c), r);
// }


// template <typename L>
// auto operator+(L l, double c) {
//     return Add<L, Const>(l, Const(c));
// }

// template <typename L>
// auto operator+(L l, int c) {
//     return Add<L, Const>(l, Const(c));
// }
