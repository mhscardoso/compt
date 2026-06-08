// Banca de Testes

#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <type_traits>

#include "lib.h"

 
Var print( const Var& o ) {
    cout << "{ nome: " << o["nome"]
        << ", idade: " << o["idade"]( o )
        << ", nascimento: " << o["nascimento"]
        << ", print: " << o["print"]
        << ", atr: " << o["atr"] << " }" << endl;
    return Var();
}
 
void imprime( Var v ) { v["print"]( v ); }



int main(void) try {

#ifdef teste1
    Var a, b = 10;
    cout << a << " " << b << endl;
    a = 3.14;
    b = "uma string";
    cout << a << " " << b << endl;


#elif defined(teste2)
    Var a;
    a = 3;
    cout << a["mes"] << endl;

#elif defined(teste3)
    Var a;
    a = newObject();
    cout << a(5) << endl;


#elif defined(teste4)
    Var a[5] = { true, 'X', 2, 2.1, "abracadabra" };
    Var b = 200, c = 300.1, d = "palavra ";
    for( auto& x: a ) {
      cout << x+b << "," << x+c << "," << x+d << "," << 3 / x << "," << 1.1 * x << "," 
           << Var(x && true) << "," << Var(x && false) <<  "," << Var(b >= x) << "," << Var(x < d) << endl;
    }


#elif defined(teste5)
    Var a, b;
    a = newObject();
    b = "José Maria";
    a["nome"] = b;
    a["nascimento"] = 1998;
    b = "Maria José";
    try {
      print( a );
    } catch( Var::Erro e ) {
      cout << "Erro fatal: " << e() << endl;
    }
    cout << a << " " << a["nome"] << " " << a["nascimento"] << endl;


#elif defined(teste6)
    Var a, b;
    a = 10.1;
    b = []( auto x ){ return x + x; };
    cout << b( a ) << " ";
    cout << b( "oba" ) << " ";
    cout << b( 'X' ) << " ";
    cout << b( true );


#elif defined(teste7)
    Var a = newObject();
    Var b = "José", c = "Maria";
    a["nome"] = b + ' ' + c;
    a["idade"] = []( auto v ) { return 2019 - v["nascimento"]; };
    a["nascimento"] = 1990;
    a["print"] = &print;
    b = a;
    imprime( a );
    a["nascimento"] = 2001;
    imprime( a );
    imprime( b );


#elif defined(teste8)
    Var a, b;
    a = newObject();
    a["init"] = []( auto x ) { x["nome"] = "Manoel"; 
                               x["idade"] = []( auto v ) { return 2019 - v["nascimento"]; }; 
                               x["nascimento"] = 1987; 
                               x["print"] = &print;
                               return x; };
    b = [a]( auto x ){ return x( a ); };
    a["funcao"] = b;
    b = &print;
    a["funcao"]( a["init"] );
    a["atr"] = a["init"]( newObject() ); 
    a["funcao"]( b );
    imprime( a["atr"] );

#endif

} catch( Var::Erro e ) {
    cout << "Erro fatal: " << e() << endl;
}
