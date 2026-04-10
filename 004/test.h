// Banca de Testes


#include "../tests.h"


TT(teste1) {
	print( cout, { { "jan", 1 }, { string( "pi" ), 3.14 } } );
}

TT(teste2) {
	print( cout, { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );
}


TT(teste3) {
	print( cout, { { "jan", 1 }, { 2, "fev" }, { "lista", vector<int>{ 1, 2, 3 } }, { 1, 2 }, { 1.1, 2.2} } );
}



TT(teste4) {
	Pair p{ "1", "2" };
	print( cout, { { "jan", 1 }, { string( "pi" ), 3.14 }, p } );
}


TT(teste5) {
	stringstream buf;
	print( buf, { { "jan", 1 }, { 2, "fev" }, { "lista", vector<int>{ 1, 2, 3 } }, { 1, 2 }, { 1.1, 2.2} } );
	cout << "Inicio\n" << buf.str() << "Fim" << endl;
}
