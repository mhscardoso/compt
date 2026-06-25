// Banca de Testes

#include "lib.h"

using namespace std;

int main(void) try {

#ifdef teste1
    // Eu criei uma função apply com minúsculas.
    // template <typename F> Apply<F> apply( F f ) { return Apply<F>(f); }
    // Sua classe deve ser Apply, com maiúscula.
    int i = 0;
    auto f0 = apply( [&i]( double x ) { return i++; } );
    auto print = apply( []( double x ) { cout << x << " "; return x; } );
    auto f2 = apply( []( double x ) { return x*x; } );
    Matriz<4,3> m;

    m = f0( m );
    print( m );
    cout << endl;
    m = f2( m );
    print( m );
    cout << endl;

    cout << tamanho( m ) << endl;


#elif defined(teste2)
    Matriz<100,1> a;
    Matriz<1,100> b;

    auto x = a * b;
    auto y = b * a;

    cout << tamanho( x ) << endl;
    cout << tamanho( y ) << endl;

#elif defined(teste3)
    Matriz<17,93> a, b;

    auto x = a + b;
    auto y = a + b + a + b;

    cout << tamanho( x ) << endl;
    cout << tamanho( y ) << endl;


#elif defined(teste4)
    Matriz<1000,1> a;
    Matriz<1,1000> b;
    Matriz<1000,1000> c;
    int tempo = 0, lapso1, lapso2;

    tempo = clock();

    Matriz<1000,1000> x1 = a * b;
    Matriz<1000,1000> x2 = x1 * c;


    cout << tamanho(x1) << endl;
    cout << tamanho(x2) << endl;

    lapso1 = clock() - tempo;

    tempo = clock();

    Matriz<1000,1000> y = a * b * c;

    lapso2 = clock() - tempo;
    cout << tamanho(y) << endl;

    if( lapso1/lapso2 > 10 ) 
        cout << "Otimizou" << endl;
    else
        cout << "Não otimizou" << endl; 


#elif defined(teste5)
    Matriz<997,1> a;
    Matriz<1,997> b;
    Matriz<997,997> c;
    int tempo = 0, lapso1, lapso2;

    tempo = clock();

    Matriz<997,997> x1 = a * b;
    Matriz<997,997> x2 = x1 * c;
    Matriz<997,997> t1 = a * (b + b);
    Matriz<997,997> x3 = x2 + t1 * c;

    cout << tamanho(x1) << endl;
    cout << tamanho(x2) << endl;
    cout << tamanho(t1) << endl;
    cout << tamanho(x3) << endl;

    lapso1 = clock() - tempo;

    tempo = clock();

    Matriz<997,997> y = a * b * c + a * (b + b) * c;

    lapso2 = clock() - tempo;
    cout << tamanho(y) << endl;

    if( lapso1/lapso2 > 10 ) 
        cout << "Otimizou" << endl;
    else
        cout << "Não otimizou" << endl; 


#endif

} catch(...) {
    cout << "Erro fatal: " << endl;
}
