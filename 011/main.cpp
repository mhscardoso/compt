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

int main(void) {

#ifdef teste1

    variant<int,char,string> v = 42;
    cout << v << "\n";



#elif defined(teste2)
    variant<int,char,string> v = 'Z';
    cout << v << "\n";



#elif defined(teste3)
    variant<int,double,string> v = 3.14;
    cout << v << "\n";



#elif defined(teste4)
    variant<int,string,double> v = 7;
    cout << v << " ";
    v = string("depois");
    cout << v << "\n";



#elif defined(teste5)
    variant<string,int,double> v = string("Hello, world");
    cout << v << "!";



#elif defined(teste6)
    variant<string,int,double> v = string("primeiro");
    cout << v;



#elif defined(teste7)
    variant<int,string,double> v = 2.5;
    cout << v;



#elif defined(teste8)
    variant<int,char,string,double,float,long> v = 3.14f;
    cout << v;



#elif defined(teste9)
    variant<int,string,double> v = 42;
    cout << v << endl;


#elif defined(teste10)
    vector<variant<int,double,string>> v = {1, 3.14, string("ola"), 42, string("mundo")};
    for (auto& x : v) cout << x << endl;

#endif

}
