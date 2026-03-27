using namespace std;

const int MAX_PILHA = 100;

class PilhaInt {
private:
	int arr[MAX_PILHA];
	int topo;

public:

	PilhaInt(): topo(0) {}

	int getTopo() const {
		return topo;
	}

	int get(int i) const {
		return arr[i];
	}

	void empilha(int elemento) {
		if (topo >= MAX_PILHA) {
			cout << "Pilha Cheia" << endl;
			return;
		}

		arr[topo++] = elemento;
	}

	int desempilha() {
		if (topo <= 0) {
			cout << "Pilha Vazia" << endl;
			return -1;
		}

		return arr[--topo];
	}

	PilhaInt& operator<<(const int elemento) {
		this->empilha(elemento);

		return *this;
	}

	void print(ostream& os) const {
		os << "[";
		for (int i = 0; i < topo; i++) {
			os << " " << arr[i];
			if (i < topo - 1) {
				os << ",";
			}
		}
		os << " ]";
	}

	PilhaInt& operator=(const PilhaInt& p) {

		while (this->topo > 0) {
			this->desempilha();
		}

		for (int i = 0; i < p.getTopo(); i++) {
			this->empilha(p.get(i));
		}

		return *this;
	}
};


void teste1() {
	PilhaInt p;
	p.empilha( 1 );
	p.empilha( 2 );
	cout << p.desempilha() << endl;
	cout << p.desempilha() << endl;
}

void teste2() {
	PilhaInt p;
	p << 19 << 18 << 17;
	p << 30;
	cout << p.desempilha() << "," << p.desempilha() << "," 
		<< p.desempilha() << "," << p.desempilha() << endl;
}


void teste3() {
	PilhaInt p;
	p << 19 << 18 << 17 << 30;

	p.print( cout );
	cout << endl;
}



void teste4() {
	PilhaInt p;
	p << 19 << 18 << 17 << 30;

	stringstream ss;
	p.print( ss );

	cout << "{" << ss.str() << "}" << endl;
}


void teste5() {
	PilhaInt p, q;
	q << 2;
	p << 19 << 18 << 17 << 30;
	q = p;
	p.desempilha();
	q << 7;
	stringstream ssp, ssq;
	p.print( ssp );
	q.print( ssq );

	cout << "q = " << ssq.str() << "\n" << "p = " << ssp.str() << endl;
}
