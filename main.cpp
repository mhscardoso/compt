#include <initializer_list>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

// Define folder and mode
#define WORK  004
#define JUDGE 0


#define STRINGFY(x) #x
#define EXPAND(x)   STRINGFY(x)
#define TEST_FILE   EXPAND(WORK/expected.txt)

#include EXPAND(WORK/lib.h)
#include EXPAND(WORK/test.h)


int main(void) {

#if JUDGE == 0

    double NOTA = 0;
	std::ios::sync_with_stdio(true);

	divide();
	divide();

    auto esperados = ler_resultados(TEST_FILE);

    for (auto& t : testes) {
        string output = capture_output(t.func);

        string esperado;
        
        try {
            esperado = esperados.at(t.nome);
        } catch (...) {
            cout << t.nome << ": SEM RESULTADO ESPERADO\n";
            continue;
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

    divide();
    double nota = (NOTA / testes.size()) * 100;

    cout << "Nota Final: " << setprecision(4) << nota << endl;

    divide();

#else

    testes.at(JUDGE-1).func();

#endif // JUDGE == ?

}
