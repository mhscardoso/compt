using namespace std;

const int MAX_PILHA = 100;

class PilhaInt {
private:
	int topo;
	int tamanho;
	int *ptr_arr;

public:

	PilhaInt(int tamanho = 10): topo(0), tamanho(tamanho) {
		int *arr = (int*) malloc(tamanho * sizeof(int));

		if (arr == NULL) {
			cout << "Falhou a alocação de memória" << endl;
			free(ptr_arr);
			exit(1);
		}

		ptr_arr = arr;
	}

	PilhaInt(const PilhaInt& p): topo(p.topo), tamanho(p.tamanho) {
		int *arr = (int*) malloc(tamanho * sizeof(int));

		if (arr == NULL) {
			cout << "Falhou a alocação de memória" << endl;
			exit(1);
		}

		for (int i = 0; i < topo; i++) {
			arr[i] = p.get(i);
		}

		ptr_arr = arr;
	}

	~PilhaInt() {
		free(ptr_arr);
	}

	int getTopo() const {
		return topo;
	}

	int get(int i) const {
		return ptr_arr[i];
	}

	int capacidade() const {
		return tamanho;
	}

	void redimensiona(int n) {
		if (n == tamanho) {
			return;
		}

		int *tmp = (int *) realloc(ptr_arr, n * sizeof(int));

		if (tmp == NULL) {
			cout << "Teste Falhou" << endl;
			free(ptr_arr);
			return;
		}

		ptr_arr = tmp;
		tamanho = n;
	}

	void empilha(int elemento) {
		if (topo == tamanho) {
			this->redimensiona(tamanho * 2);
			tamanho = tamanho * 2;
		}

		ptr_arr[topo++] = elemento;
	}

	int desempilha() {
		if (topo <= 0) {
			cout << "Pilha Vazia" << endl;
			return -1;
		}

		return ptr_arr[--topo];
	}

	PilhaInt& operator<<(const int elemento) {
		this->empilha(elemento);

		return *this;
	}

	void print(ostream& os) const {
		os << "[";
		for (int i = 0; i < topo; i++) {
			os << " " << ptr_arr[i];
			if (i < topo - 1) {
				os << ",";
			}
		}
		os << " ]";
	}

	PilhaInt& operator=(const PilhaInt& p) {
		if (this == &p) { return *this; }

		while (this->topo > 0) {
			this->desempilha();
		}

		int nova_capacidade = p.capacidade();

		this->redimensiona(nova_capacidade);

		for (int i = 0; i < p.getTopo(); i++) {
			this->empilha(p.get(i));
		}

		return *this;
	}
};


PilhaInt embaralha( PilhaInt q ) {
	int aux = q.desempilha();
	q << 32 << 9 << aux;
	
	return q;
}


void teste1() {
	PilhaInt p;
	p.empilha( 1 );
	p << 3 << 9 << 13 << 89;
	cout << p.desempilha() << endl;
	cout << p.desempilha() << endl;
	p.print( cout );
	p << 19 << 18 << 17 << 30;
	stringstream ss;
	p.print( ss );
	cout << endl << "{" << ss.str() << "}" << endl;
}

void teste2() {
	PilhaInt a(5), b(15);
	cout << a.capacidade() << endl;
	cout << b.capacidade() << endl;
}


void teste3() {
	PilhaInt a( 7 );
	a << 3 << 5 << 13 << 19;
	PilhaInt b{ embaralha( a ) };
	a.print( cout ); cout << endl;
	b.print( cout ); cout << endl;
}



void teste4() {
	PilhaInt a{7}, b{500}, c{5};
	a << 8 << 3 << 1 << 4 << 5;
	b << 1 << 2 << 3;
	c = a;
	a = b;
	b = c;
	c.desempilha();
	c << 7;
	a.print( cout ); cout << endl;
	b.print( cout ); cout << endl;
	c.print( cout ); cout << endl;
}


void teste5() {
	PilhaInt a{7}, b{500000}, c{5};
	a << 8 << 3 << 1 << 4 << 5;
	for( int i = 0; i < b.capacidade(); i++ )
		b << i;
	c = a;
	a = b;
	b = c;
	cout << a.capacidade() << ", " << b.capacidade() << ", " << c.capacidade() << endl;
}


void teste6() {
	PilhaInt a{7};
	a << 8 << 3 << 1 << 4 << 5;
	a = a;
	a.print( cout ); cout << endl;
}

void teste7() {
	PilhaInt a{81};
	a << 5 << 6 << 3 << 2 << 9 << 13;
	a.redimensiona( 81 ); cout << a.capacidade() << endl;
	a.redimensiona( 11 ); cout << a.capacidade() << endl;
	a.redimensiona( 6 ); cout << a.capacidade() << endl;
	a.print( cout ); cout << endl;
	a.redimensiona( 3 ); cout << a.capacidade() << endl;
	a.print( cout ); cout << endl;
}

void teste8() {
	PilhaInt a{3};
	for( int i = 0; i < 20; i++ ) {
		a << i;
		cout << a.capacidade() << " ";
	}
	cout << endl;
	a.print( cout ); cout << endl;
}
