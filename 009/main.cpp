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


template <typename T> 
void myPrint( T t ) {
    cout << t << " ";
}

struct PrintM {
  void operator() ( vector<int> v ) {
      cout << "[ " << v[0] << ", " << v[1] << " ] ";
  }
};

struct PrintM2 {
  void operator() ( vector<int> v ) {
      v | myPrint<int>;
  }
};

string StringToUpper(string strToConvert)
{
   for (std::string::iterator p = strToConvert.begin(); strToConvert.end() != p; ++p)
       *p = toupper(*p);

   return strToConvert;
}

struct PrintM3 {
    void operator()( string st ) {
        cout << StringToUpper( st ) << " ";
    }
};


int main(void) {

#ifdef teste1
	vector<int> v1 = { 2, 9, 8, 8 };
    vector<string> v2 = { "jose", "casado" };
    vector<double> v3 = { 3.11, 5, 7.9 };
    
    v1 | []( int n ){ cout << n << " "; };
    cout << endl;
    v2 | []( string n ){ cout << n << " "; };
    cout << endl;
    v3 | []( double n ){ cout << n << " "; };
    cout << endl;


#elif defined(teste2)
	int tabInt[] = { 1, 5, 9, 3 };
	double tabDouble[] = { 3.1, 5.3, 9 };
	string tabString[] = { "a", "e", "i", "o", "u", "ypsilon" };

	tabInt | []( int n ){ cout << n << " "; };
	cout << endl;
	tabDouble | []( double n ){ cout << n << " "; };
	cout << endl;
	tabString | []( string n ){ cout << n << " "; };
	cout << endl;



#elif defined(teste3)
    vector<int> v1 = { 1, 9, 5, 8 };
    vector<string> v2 = { "joao", "Solteiro" };
    vector<double> v3 = { 3.14, 999, 7.9, 900 };
    
    v1 | myPrint<int>;
    cout << endl;
    v2 | myPrint<string>;
    cout << endl;
    v3 |  myPrint<double>;
    cout << endl;




#elif defined(teste4)
    vector<vector<int>> m = { { 1, 2 }, { 4, 6 }, { 6, 7 } };
    string tab[] = { "hello,", "world" };
    
    m | PrintM();
    cout << endl;
    m | PrintM2();
    cout << endl;
    tab | PrintM3();
    cout << endl;



#elif defined(teste5)
    auto x = { 1, 2, 3 };
    x | []( int x ) { cout << x*x << " "; };
    cout << endl;



#elif defined(teste6)
    vector<int> v = { 1, 2, 3, 4, 5, 6 };
    
    v | []( int x ) { return x % 2 == 0; }
      | []( int x ) { cout << x << endl; };



#elif defined(teste7)
    vector<int> v = { 1, 2, 3, 4, 5, 6 };
    
    v | []( int x ) { return x % 2 == 0; }
      | []( int x ) { return x * 4;      }
      | []( int x ) { cout << x << endl; };



#elif defined(teste8)
    vector<int> v1 = { 2, 9, 8, 8, 7, 4 };
    auto result = v1 | []( int x ) { return x % 2 == 0; };
    for( auto x : result ) 
        cout << x << " ";

    cout << endl;



#elif defined(teste9)
    vector<int> v1 = { 2, 9, 8, 8, 7, 4 };
    v1 | []( int x ) { return x % 2 == 0; } | [] ( int x ) { cout << x << " "; };

    cout << endl;



#elif defined(teste10)
    int v1[] = { 2, 9, 8, 8, 7, 4 };
    auto result = v1 | []( int x ) { return x % 2 == 0; };
    for( auto x : result ) 
        cout << x << " ";

    cout << endl;



#elif defined(teste11)
    int v1[] = { 2, 9, 8, 8, 7, 4 };
    v1 | []( int x ) { return x % 2 == 0; } | [] ( int x ) { cout << x << " "; };

    cout << endl;



#elif defined(teste12)
    map<string,string> v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" } };
    v | []( auto x ){ return pair{ x.first, stod( x.second ) }; } | []( auto p ) { cout << p.second + 1.1 << " "; };

    cout << endl;


#elif defined(teste13)
    map<string,string> v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" }, { "f", "6" } };
    auto par = []( auto x ){ return stoi( x.second ) % 2 == 0; };
    auto upper_first = []( auto x ){ string st = x.first; transform(st.begin(), st.end(), st.begin(), ::toupper); return st; };
    v | par | upper_first | []( auto st ) { cout << st + "! "; };
    cout << endl;



#elif defined(teste14)
    map<string,string> v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" }, { "F", "6" } };
    auto par = []( auto x ){ return stoi( x.second ) % 2 == 0; };
    auto first = []( pair<string,string> p ) { return p.first; };
    auto upper = []( string st ){ transform(st.begin(), st.end(), st.begin(), ::toupper); return st; };
    vector<string> result = v | par | first | upper; 
    result | []( auto st ) { cout << st + "! "; };
    cout << endl;



#elif defined(teste15)
    vector<string> v1 = { "janeiro", "fevereiro", "março", "abril", "maio" };
    v1 | &string::length | [] ( int x ) { cout << x << " "; };
    cout << endl;

#endif

}
