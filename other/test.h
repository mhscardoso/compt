// Banca de Testes


#include "../tests.h"

Pilha<int> embaralha( Pilha<int> q ) {
	int aux = q.desempilha();
	q << 32 << 9 << aux;
	
	return q;
}


TT(teste1) {
	Pilha<int> p;
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

TT(teste2) {
	Pilha<int> a(5), b(15);
	cout << a.capacidade() << endl;
	cout << b.capacidade() << endl;
}


TT(teste3) {
	Pilha<int> a( 7 );
	a << 3 << 5 << 13 << 19;
	Pilha<int> b{ embaralha( a ) };
	a.print( cout ); cout << endl;
	b.print( cout ); cout << endl;
}



TT(teste4) {
	Pilha<int> a{7}, b{500}, c{5};
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


TT(teste5) {
	Pilha<int> a{7}, b{500000}, c{5};
	a << 8 << 3 << 1 << 4 << 5;
	for( int i = 0; i < b.capacidade(); i++ )
		b << i;
	c = a;
	a = b;
	b = c;
	cout << a.capacidade() << ", " << b.capacidade() << ", " << c.capacidade() << endl;
}


TT(teste6) {
	Pilha<int> a{7};
	a << 8 << 3 << 1 << 4 << 5;
	a = a;
	a.print( cout ); cout << endl;
}

TT(teste7) {
	Pilha<int> a{81};
	a << 5 << 6 << 3 << 2 << 9 << 13;
	a.redimensiona( 81 ); cout << a.capacidade() << endl;
	a.redimensiona( 11 ); cout << a.capacidade() << endl;
	a.redimensiona( 6 ); cout << a.capacidade() << endl;
	a.print( cout ); cout << endl;
	a.redimensiona( 3 ); cout << a.capacidade() << endl;
	a.print( cout ); cout << endl;
}

TT(teste8) {
	Pilha<int> a{3};
	for( int i = 0; i < 20; i++ ) {
		a << i;
		cout << a.capacidade() << " ";
	}
	cout << endl;
	a.print( cout ); cout << endl;
}
