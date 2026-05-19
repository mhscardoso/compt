// Banca de Testes

#include <map>
#include <cmath>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <initializer_list>

#include "lib.h"

int main(void) {

#ifdef teste1
    double v = 5.1;
    auto f = 3.0 * x * x;

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;



#elif defined(teste2)
    double v = 1.1;
    auto f = x*x*( x + 8.0 ) + x;

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;



#elif defined(teste3)
    double v = 0.5;
    auto f = sin( x * x * cos( 3.14 * x + 1.0 ) );

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;



#elif defined(teste4)
    double v = 3.14;
    auto f = sin( x * x - cos( 3.14 * x + 1.0 ) );

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;



#elif defined(teste5)
    double v = 3.14159;
    auto f =  sin(x)/cos(x);

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;



#elif defined(teste6)
    double v = -7.3;
    auto f = x->*3 + x->*2;

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;



#elif defined(teste7)
    double v = -0.5;
    auto f = 1.0 / (sin(x)->*2 + cos(x)->*2)->*4;

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;



#elif defined(teste8)
    double v = 0.1;
    auto f = 1 / (1 + exp( -2*( x - 1 )->*4 ) );

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;



#elif defined(teste9)
    double v = 3;
    auto f = exp( x * log( x ) );

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;



#elif defined(teste10)
    double v = 3;
    auto f =  2 * x->*1.1;

    cout << "f(" << v << ")=" << f.e(v) << ", f'(" << v << ")=" << f.dx(v) << endl;

#endif

}
