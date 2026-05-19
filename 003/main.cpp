// Banca de Testes

#include <vector>
#include <iostream>
#include <sstream>

#include "lib.h"


PilhaInt embaralha( PilhaInt q ) {
	int aux = q.desempilha();
	q << 32 << 9 << aux;
	
	return q;
}


int main(void) {

#ifdef teste1
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



#elif defined(teste2)
	PilhaInt a(5), b(15);
	cout << a.capacidade() << endl;
	cout << b.capacidade() << endl;



#elif defined(teste3)
	PilhaInt a( 7 );
	a << 3 << 5 << 13 << 19;
	PilhaInt b{ embaralha( a ) };
	a.print( cout ); cout << endl;
	b.print( cout ); cout << endl;




#elif defined(teste4)
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



#elif defined(teste5)
	PilhaInt a{7}, b{500000}, c{5};
	a << 8 << 3 << 1 << 4 << 5;
	for( int i = 0; i < b.capacidade(); i++ )
		b << i;
	c = a;
	a = b;
	b = c;
	cout << a.capacidade() << ", " << b.capacidade() << ", " << c.capacidade() << endl;



#elif defined(teste6)
	PilhaInt a{7};
	a << 8 << 3 << 1 << 4 << 5;
	a = a;
	a.print( cout ); cout << endl;



#elif defined(teste7)
	PilhaInt a{81};
	a << 5 << 6 << 3 << 2 << 9 << 13;
	a.redimensiona( 81 ); cout << a.capacidade() << endl;
	a.redimensiona( 11 ); cout << a.capacidade() << endl;
	a.redimensiona( 6 ); cout << a.capacidade() << endl;
	a.print( cout ); cout << endl;
	a.redimensiona( 3 ); cout << a.capacidade() << endl;
	a.print( cout ); cout << endl;



#elif defined(teste8)
	PilhaInt a{3};
	for( int i = 0; i < 20; i++ ) {
		a << i;
		cout << a.capacidade() << " ";
	}
	cout << endl;
	a.print( cout ); cout << endl;

#endif

}
