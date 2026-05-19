// Banca de Testes

#include <map>
#include <cmath>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>

#include "lib.h"


int main(void) {

#ifdef teste1
	print( cout, { { "jan", 1 }, { string( "pi" ), 3.14 } } );


#elif defined(teste2)
	print( cout, { { "jan", 1 }, { 2, "fev" }, { string( "pi" ), 3.14 } } );



#elif defined(teste3)
	print( cout, { { "jan", 1 }, { 2, "fev" }, { "lista", vector<int>{ 1, 2, 3 } }, { 1, 2 }, { 1.1, 2.2} } );




#elif defined(teste4)
	Pair p{ "1", "2" };
	print( cout, { { "jan", 1 }, { string( "pi" ), 3.14 }, p } );



#elif defined(teste5)
	stringstream buf;
	print( buf, { { "jan", 1 }, { 2, "fev" }, { "lista", vector<int>{ 1, 2, 3 } }, { 1, 2 }, { 1.1, 2.2} } );
	cout << "Inicio\n" << buf.str() << "Fim" << endl;

#endif

}
