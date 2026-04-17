// Banca de Testes


#include "../tests.h"


TT(teste1) {
	Vetor<3, double> a;

	a = { 4, 5, 6 };

	cout << a << endl;
}

TT(teste2) {
	Vetor<3, double> a, b;

	a = { 4, 5, 6 };
	b = { 1, 2, 3 };

	cout << a + b << endl;
}


TT(teste3) {
	Vetor<3, double> a, b;

	a = { 4, 5, 6 };
	b = { 1, 2, 3 };

	Vetor<3, double> c;
	c = a ** b; 
	cout << c << endl; 
}


TT(teste4) {
	Vetor<3, double> a, b;

	a = { 4, 5, 6 };
	b = { 1, 2, 3 };

	cout << a * b << endl;
}


TT(teste5) {
	Vetor<3, double> a;

	a = { 4, 5, 6 };

	cout << a * 3.0 << endl;
}


TT(teste6) {
	Vetor<3, double> a;

	a = { 4, 5, 6 };

	cout << 2.1 * a << endl;
}


TT(teste7) {
	Vetor<3, double> a, b;

	a = { 4, 5, 6 };
	b = { 1, 2, 3 };

	cout << a + b + a * 2.0 << endl;
}
