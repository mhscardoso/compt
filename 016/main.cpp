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
#include <initializer_list>

#include "lib.h"

vector<int> operator + ( const vector<int>& v, int n ) {
    vector<int> result{ v };
    result.push_back( n );

    return result;
}

int main(void) {

    decltype(::x) x(::x);

#ifdef teste1

vector< vector<int> > m{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
m | []( auto v ) { return v[0] != 0; } | [x]( auto v ) { v + 7 | cout << x << ' '; };


#elif defined(teste2)

vector< vector<int> > m{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
m | []( auto v ) { return v[0] != 0; } | ( x | cout << x << ' ' );


#elif defined(teste3)

vector< vector<int> > m{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
m | x[0] != 0 | ( x | cout << x << ' ' );


#elif defined(teste4)

vector< vector<int> > m{ { 1, 2, 3 }, { 0, 3, 7 }, { 1, 3 } };
m | x[0] != 0 | (( x + 7 ) | cout << x << ' ' );


#endif

}
