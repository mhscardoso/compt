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

    int arr[] = { 1, 2, 3, 4, 5 };
    arr | cout << x*x << ' ';



#elif defined(teste2)
    vector<int> v = { 1, 2, 3, 4, 5, 6, 7 };
    v | (x % 2 == 1) | cout << x << ' ';



#elif defined(teste3)
    vector<int> v = { 3, 5, 7 };
    v | (x + x + x) | cout << x << ' ';



#elif defined(teste4)
    vector<int> v;
    int arr[] = { 2, 3, 4, 5, 6 };
    arr | [&v]( int n ) { v.push_back( n ); };
    v | (x % 2 == 0) | cout << x << ' ';



#elif defined(teste5)
    vector<int> v = { 1, 2, 3, 4, 5, 6 };
    v | (x % 2 == 0) | x*x | cout << x << ' ';



#elif defined(teste6)
    vector<int> v = { 10, 20, 30 };
    v | (x + 1) | cout << x << ' ';



#elif defined(teste7)
    string arr[] = { "foo", "bar", "baz" };
    arr | (x + x) | cout << x << ' ';



#elif defined(teste8)
    map<string,int> m = { {"a",1}, {"b",2}, {"c",3} };
    m | []( auto p ) { cout << p.first << ":" << p.second << ' '; };



#elif defined(teste9)
    vector<vector<int>> vv = { {1,2,3}, {4,5,6} };
    vv | []( vector<int> inner ) { inner | cout << x*x << ' '; cout << "| "; };


#elif defined(teste10)
    vector<vector<int>> vv = { {1,2,3,4,5}, {6,7,8,9,10} };
    vv | []( vector<int> inner ) { inner | []( int n ) { return n % 2 == 0; } | cout << x << ' '; };

#endif

}
