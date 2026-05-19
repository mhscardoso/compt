// Banca de Testes

#include <map>
#include <cmath>
#include <vector>
#include <sstream>
#include <iostream>

#include "lib.h"


int main(void) {

#ifdef teste1
	Vetor<3, double> a;

	a = { 4, 5, 6 };

	cout << a << endl;


#elif defined(teste2)
	Vetor<3, double> a, b;

	a = { 4, 5, 6 };
	b = { 1, 2, 3 };

	cout << a + b << endl;



#elif defined(teste3)
	Vetor<3, double> a, b;

	a = { 4, 5, 6 };
	b = { 1, 2, 3 };

	Vetor<3, double> c;
	c = a ** b; 
	cout << c << endl; 



#elif defined(teste4)
	Vetor<3, double> a, b;

	a = { 4, 5, 6 };
	b = { 1, 2, 3 };

	cout << a * b << endl;



#elif defined(teste5)
	Vetor<3, double> a;

	a = { 4, 5, 6 };

	cout << a * 3.0 << endl;



#elif defined(teste6)
	Vetor<3, double> a;

	a = { 4, 5, 6 };

	cout << 2.1 * a << endl;



#elif defined(teste7)
	Vetor<3, double> a, b;

	a = { 4, 5, 6 };
	b = { 1, 2, 3 };

	cout << a + b + a * 2.0 << endl;

#endif

}
