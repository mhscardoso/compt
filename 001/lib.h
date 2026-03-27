#include <iostream>

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


	friend ostream& operator<<(ostream& os, const PilhaInt& p);


	void show() {
		cout << "[";
		for (int i = 0; i < topo; i++) {
			cout << " " << arr[i];
		}
		cout << " ]" << endl;
	}
};


ostream& operator<<(ostream& os, const PilhaInt& p) {
	os << "[";
	for (int i = 0; i < p.getTopo(); i++) {
		os << " " << p.get(i);
	}
	os << " ]";

	return os;
}
