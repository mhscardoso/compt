// Banca de Testes

#include "../tests.h"


TT(teste1) {
    auto f = 3.0 * x * x;

    cout << f.v(2) << endl;
    cout << f.dx(2) << endl;

    f = f + 5;

    cout << f.v(2) << endl;
    cout << f.dx(2) << endl;
}
