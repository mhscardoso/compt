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
