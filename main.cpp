#include <initializer_list>
#include <type_traits>
#include <functional>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <math.h>
#include <string>
#include <vector>
#include <map>

// Define folder and mode
// #define WORK  007
// #define JUDGE 0

double NOTA = 0;

#define STRINGFY(x) #x
#define EXPAND(x)   STRINGFY(x)
#define TEST_FILE   EXPAND(WORK/expected.txt)

#include EXPAND(WORK/lib.h)
#include EXPAND(WORK/test.h)

void execute_atomic_test(
    const TestCase& t, 
    const std::unordered_map<std::string, std::string>& esperados
) {
    string output = capture_output(t.func);

    string esperado;
    
    try {
        esperado = esperados.at(t.nome);
    } catch (...) {
        cout << t.nome << ": SEM RESULTADO ESPERADO\n";
        return;
    }

    trim_final(esperado);
    trim_final(output);

    if (output == esperado) {
        cout << t.nome << ": OK\n";
        NOTA++;
    } else {
        cout << t.nome << ": FAIL\n";
        cout << "Esperado:\n" << esperado << endl;
        cout << "Obtido:\n" << output << endl;
    }

    divide();
}


int main(void) {

	std::ios::sync_with_stdio(true);

	divide();
	divide();

    auto esperados = ler_resultados(TEST_FILE);

#if JUDGE == 0

    for (auto& t : testes) {
        execute_atomic_test(t, esperados);
    }

    divide();
    double nota = (NOTA / testes.size()) * 100;

    cout << "Nota Final: " 
         << setprecision(4) << nota 
         << endl;

    divide();

#else

    auto t = testes.at(JUDGE-1);
    execute_atomic_test(t, esperados);

#endif // JUDGE == 0

    /////////////// ESPECÍFICO PARA O TRABALHO 010 - DERIVADOR SIMBÓLICO PARTE 1 ///////////////

    // cout << "teste1" << endl;
    // teste1();

    // cout << endl;

    // cout << "teste2" << endl;
    // teste2();

    // cout << endl;

    // cout << "teste3" << endl;
    // teste3();

    // cout << endl;

    // cout << "teste4" << endl;
    // teste4();

    // cout << endl;

    // cout << "teste5" << endl;
    // teste5();

    // cout << endl;

    // cout << "teste6" << endl;
    // teste6();

    // cout << endl;

    // cout << "teste7" << endl;
    // teste7();

    // cout << endl;

    // cout << "teste8" << endl;
    // teste8();

    // cout << endl;

    // cout << "teste9" << endl;
    // teste9();

    // cout << endl;

    // cout << "teste10" << endl;
    // teste10();

}
