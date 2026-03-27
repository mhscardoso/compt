#include "001/lib.h"

int main(void) {
	PilhaInt p;

	p << 2 << 7 << 10 << 9238 << 6;

	cout << p << endl;

	int e = p.desempilha();
	cout << "Desempilhou " << e << endl;

	e = p.desempilha();
	cout << "Desempilhou " << e << endl;

	cout << p << endl;
}
