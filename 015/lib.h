using namespace std;

class Object {
public:
	virtual ~Object() = default;
	virtual void print(ostream& o) const = 0;
	virtual unique_ptr<Object> clone() const = 0;
};


template <typename Derived, typename T>
class Undefined : public Object {
private:
	T value;

public:
	Undefined(T v): value(move(v)) {}

	unique_ptr<Object> clone() const override {
		unique_ptr<Object> result{ new Derived(static_cast<const Derived&>(*this)) };

		return result;
	}

	void print(ostream& o) const override {
		o << value;
	}

	T get() const {
		return value;
	}
};



class Int     : public Undefined<Int,    int>    { public: using Undefined::Undefined; };
class Double  : public Undefined<Double, double> { public: using Undefined::Undefined; };
class String  : public Undefined<String, string> { public: using Undefined::Undefined; };
class Boolean : public Undefined<Boolean, bool>  {
public: 
	using Undefined::Undefined;

	void print(ostream& o) const override {
		string result = get() ? "true" : "false";
		o << result;
	}
};


class Var {
private:
	unique_ptr<Object> obj;

public:
	Var(int           value) : obj(new Int(value))     {}
	Var(bool          value) : obj(new Boolean(value)) {}
	Var(double        value) : obj(new Double(value))  {}
	Var(const char   *value) : obj(new String(value))  {}
	Var(const string& value) : obj(new String(value))  {}


	Var(const Var& var): obj(var.obj->clone()) {}

	Var& operator=(const Var& other_var) {
		if (this != &other_var) {
			obj = other_var.obj->clone();
		}

		return *this;
	}

	void print(ostream& o) const {
		obj->print(o);
	}

};



ostream& operator<<(ostream& o, const Var& variable) {
	variable.print(o);

	return o;
}


Var operator >  ( const Var& a, const Var& b ) { return b<a;            }
Var operator != ( const Var& a, const Var& b ) { return (a<b) || (b<a); }
Var operator == ( const Var& a, const Var& b ) { return !(a!=b);        }
Var operator <= ( const Var& a, const Var& b ) { return !(b<a);         }
Var operator >= ( const Var& a, const Var& b ) { return !(a<b);         }
