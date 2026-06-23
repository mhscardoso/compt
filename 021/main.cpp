// Banca de Testes

#include <map>
#include <cmath>
#include <vector>
#include <iostream>
#include <functional>

#include "lib.h"

int main(void) {

#ifdef teste1
    auto f = 3.0 + x + x;
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

#elif defined(teste2)
    auto f = x*x;
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

#elif defined(teste3)
    auto f = x - ( x - 5.0);
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;


#elif defined(teste4)
    auto f = (x+7.0)*(x-7.0);
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;


#elif defined(teste5)
    auto f = x->*3;
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;


#elif defined(teste6)
    auto f = (7.0*x+5.0)->*3;
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;


#elif defined(teste7)
    auto f = sin( x + 5.0 );
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;


#elif defined(teste8)
    auto f = 1.0 / (sin(x)->*2 + cos(x)->*2)->*4;
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;


#elif defined(teste9)
    auto f = exp( x * log( x - 8.0 )  + 1.0 );
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;


#elif defined(teste10)
    auto f = exp( (x + 1.0)*(x - 1.0) )->*2;
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;


#elif defined(teste11)
    auto f = log( x->* 4 + 8.0 ) * sin( cos( x / 3.14 ) );
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;


#elif defined(teste12)
    auto f = log( (8.1/(x+1.0)->*3 - 9.2 ) * (x + 3.0) *sin( cos( x / 3.14 ) ) + x );
    cout << "f(x) = " << f.str() << ", f'(x) = " << f.dx_str() << endl;

#endif

}
