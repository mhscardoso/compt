// Banca de Testes

#include <iostream>
#include <sstream>

#include "lib.h"

int main(void) {

#ifdef teste1
	PilhaInt p;
	p.empilha( 1 );
	p.empilha( 2 );
	cout << p.desempilha() << endl;
	cout << p.desempilha() << endl;


#elif defined(teste2)
	PilhaInt p;
	p << 19 << 18 << 17;
	p << 30;
	cout << p.desempilha() << "," << p.desempilha() << "," 
		<< p.desempilha() << "," << p.desempilha() << endl;



#elif defined(teste3)
	PilhaInt p;
	p << 19 << 18 << 17 << 30;

	p.print( cout );
	cout << endl;



#elif defined(teste4)
	PilhaInt p;
	p << 19 << 18 << 17 << 30;

	stringstream ss;
	p.print( ss );

	cout << "{" << ss.str() << "}" << endl;


#elif defined(teste5)
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

#endif

	return 0;

}

