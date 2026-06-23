// Banca de Testes

#include <map>
#include <cmath>
#include <vector>
#include <memory>
#include <sstream>
#include <variant>
#include <iostream>
#include <algorithm>
#include <type_traits>

#include "lib.h"


vector<int> operator+(const vector<int>& v, int n);


struct Impares {
    bool operator()(int v) { return v % 2 == 0; }
};

struct Pares {
    bool operator()(int v) { return v % 2 != 0; }
};


vector<int> operator + ( const vector<int>& v, int n ) {
    vector<int> result{ v };
    result.push_back( n );

    return result;
}

int main(void) {

    decltype(::x) x(::x);

#ifdef teste1

auto v = vector{ 0, 2, 4, 6, 8, 10 };
for( auto x : v | []( auto x ){ return x + 1; } )
  cout << x << ";";


#elif defined(teste2)
Intervalo( 9, 12 )  | []( auto x ) { cout << x << " "; };


#elif defined(teste3)
Intervalo( 9, 20 ) | Impares() | []( auto x ) { cout << x << " "; };


#elif defined(teste4)
auto st = Intervalo( 0 ) | Impares();
int n = 5;
for( auto x : st ) {
  cout << x << " ";
  if( n-- == 0 )
    break;
}


#elif defined(teste5)
auto filtro1 = []( int n ) { return n % 3 == 0; };
auto filtro2 = []( int n ) { return n % 2 == 0; };
auto st = Intervalo( 0, 30 ) | filtro1 | filtro2;
st | [](int x){ return x/10.0; } | []( auto x ) { cout << x << " "; };


#elif defined(teste6)
auto filtro1 = []( int n ) { return n % 3 == 0; };
auto filtro2 = []( int n ) { return n % 2 == 0; };
auto st = Intervalo( 0 ) | filtro1 | filtro2;
st | NPrimeiros( 10 ) | [](int x){ return x/10.0; } | []( auto x ){ cout << x << ";"; };


#elif defined(teste7)
auto toString = []( int n ){ char buf[20]; sprintf( buf, "%d", n ); return string( buf ); };
Intervalo( 1, 7 ) | toString | []( auto x ){ cout << x + "!" << ";"; };


#elif defined(teste8)
auto toString = []( int n ){ char buf[20]; sprintf( buf, "%d", n ); return string( buf ); };
vector<string> v;
Intervalo( 1 ) | Impares() | NPrimeiros( 10 ) | toString | [&v]( auto x ){ v.push_back( x ); };
v | []( auto x ){ cout << x + "0" << ";"; };


#elif defined(teste9)
string st[] = { "a", "e", "i", "o", "u" };
st | NPrimeiros( 3 ) | []( auto x ){ cout << x + "!" << ";"; };


#elif defined(teste10)
Intervalo( 9, 13 ) | Pares() | []( auto x ) { cout << x << " "; };


#endif

}
