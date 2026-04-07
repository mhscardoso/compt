#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

#define WORK 003

#define STRINGFY(x) #x
#define EXPAND(x) STRINGFY(x)

#include EXPAND(WORK/lib.h)
#include EXPAND(WORK/test.h)

double NOTA = 0;

int main(void) {
	std::ios::sync_with_stdio(true);

	divide();
	divide();

    auto esperados = ler_resultados(EXPAND(WORK/expected.txt));

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
}
