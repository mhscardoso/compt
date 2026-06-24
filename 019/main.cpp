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
    Var tab[] = { Var(), true, 10, '2', 0, '0', 'X', newObject(), "", "palavra", " 0 " };

    for( auto& a: tab )
        cout << "Valor=" << a << ":" << a.asString() << "," << a.asBool() << "," << !!a << "," << Var( a.isNumber() ) << endl;


#elif defined(teste2)
    Var a, sq = []( int n ){ return n*n; };

    a = newArray();

    for( Var b = 0; (b < 10).asBool(); b = b + 1 )
    a[b] = sq( b );

    for( int i = 0; i < 10; i++ )
    cout << a[i] << ";";

    cout << endl;

#elif defined(teste3)
    Var a = newArray(), b = 0;

    a[b] = 0;
    b[a[0]] = "a";


#elif defined(teste4)
    Var a = newArray(), b;

    a["sqr"] = []( int n ){ return n*n; }; 

    for( b = 0; (b < 10).asBool(); b = b + 1 )
    a[b] = a["sqr"]( b );

    cout << a[5];


#elif defined(teste5)
    Var a = newArray(), b = -2, c = "-2";
    a[2] = 10;
    a[b] = 5.1;
    a[a[c]] = "saida";

    cout << a[0] << "," << a[1] << "," << a[2] << "," << a[-2] << "," << a[5.1] << endl;


#elif defined(teste6)
    Var a, b;

    a = newArray();

    for( b = 0; (b < 5).asBool(); b = b + 1 )
    a[b] = b + 1;

    a.forEach( []( Var n ){ cout << n.asString() + "!"; } );
    cout << endl;


#elif defined(teste7)
    Var a, b;
    a = newArray();
    a["sqr"] = []( Var n )->Var{ return n*n; }; 

    for( b = 0; (b < 10).asBool(); b = b + 1 )
        a[b] = a["sqr"]( b );

    auto pares = []( auto n ){ return n%2 == 0; };

    a.filter( pares ).forEach( []( Var n ){ cout << n << " "; } );
    cout << endl;


#elif defined(teste8)
    Var a, b;
    a = newArray();
    Var pares = []( auto n ){ return n%2 == 0; };

    for( b = 0; (b < 10).asBool(); b = b + 1 )
        a[b] = b*b;


    auto indexOf = []( const Var& array, Var valor ) {
        int n = 0;
        int pos = -1;

        array.forEach( [&n,&pos,valor](auto x) { 
            if( pos == -1 ) { 
                if( (x == valor).asBool() ) 
                    pos = n; 
                n++; 
            } 
        } ); 

        return pos;
    };

    cout << (indexOf( a, 36 ) == a.indexOf( 36 ) ) << endl;
    cout << indexOf( a.filter( pares ), "A" ) << endl;

    a[11] = 'A';
    cout << a.indexOf( "A" ) << endl;

#endif

} catch( Var::Erro e ) {
    cout << "Erro fatal: " << e() << endl;
}
