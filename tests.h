// Código exclusivo para registrar
// e tornar possível a execução
// automatica dos testes pedidos

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <functional>
#include <unordered_map>


struct TestCase {
    std::string nome;
    std::function<void()> func;
};


std::vector<TestCase> testes;


#define TT(nome)                          \
    void nome();                             \
    struct nome##_registrador {              \
        nome##_registrador() {               \
            testes.push_back({#nome, nome}); \
        }                                    \
    } nome##_instancia;                      \
    void nome()


void divide() {
	cout << "----------------------------------" << endl;
}


class StdoutCapture {
private:
    int old_stdout;
    int pipefd[2];

public:
    StdoutCapture() {
        pipe(pipefd);

        setvbuf(stdout, NULL, _IONBF, 0);

        old_stdout = dup(STDOUT_FILENO);

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]); // importante
    }

    std::string stop() {
        fflush(stdout);
        std::cout.flush();

        // ESSENCIAL: restaurar stdout primeiro
        dup2(old_stdout, STDOUT_FILENO);
        close(old_stdout);

        // ESSENCIAL: fechar escrita implicitamente ao restaurar stdout

        std::string output;
        char buffer[1024];
        ssize_t count;

        while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output.append(buffer, count);
        }

        close(pipefd[0]);
        return output;
    }
};


std::string capture_output(std::function<void()> func) {
    StdoutCapture cap;
    func();
    return cap.stop();
}


std::unordered_map<std::string, std::string>
ler_resultados(const std::string& caminho) {
    std::ifstream file(caminho);
    std::unordered_map<std::string, std::string> resultados;

    if (!file.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivo");
    }

    std::string linha;
    std::string teste_atual;
    std::stringstream buffer;

    while (std::getline(file, linha)) {
        // Detecta início de um novo teste (linha termina com :)
        if (!linha.empty() && linha.back() == ':') {
            // Salva o anterior (se existir)
            if (!teste_atual.empty()) {
                resultados[teste_atual] = buffer.str();
                buffer.str("");
                buffer.clear();
            }

            // Remove ':' do final
            teste_atual = linha.substr(0, linha.size() - 1);
        } else {
            // Acumula conteúdo (inclusive linhas vazias!)
            buffer << linha << '\n';
        }
    }

    // Salva o último teste
    if (!teste_atual.empty()) {
        resultados[teste_atual] = buffer.str();
    }

    return resultados;
}


void trim_final(std::string& s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) {
        s.pop_back();
    }
}
