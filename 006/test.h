// Banca de Testes


#include "../tests.h"

double seno( double n ) { return sin(n); }

int id( int n ) { return n; }

string roman( int n ) {
    map<int,string> rom = { { 1, "I" }, { 2, "II" }, { 3, "III" }, { 4, "IV" }, { 5, "V" }, { 6, "VI" }, { 7, "VII" }, { 8, "VIII" } } ; 

    return rom[n]; 
}

struct FunctorSimples { 
    string operator()( int n ) { return roman( n ) + "!"; }
};

struct FunctorTemplate { 
    template <typename T>
    T operator()( T n ) { return n+n; }
};

template <typename T>
ostream& operator << ( ostream& o, const vector<T>& v ) {
    o << "[ ";
    for( auto x : v )
        o << x << " ";
        
    return o << "]";
}


TT(teste1) {
	vector<int> v{ 1, 2, 3, 4, 5 };
	vector<int> r = ::apply( v, id );
	cout << r;
}

TT(teste2) {
	vector<int> v{ 1, 2, 3, 4, 5 };
	vector<double> r = ::apply( v, seno );
	cout << r;
}


TT(teste3) {
	auto v = { 1, 2, 3, 4, 5 };
	cout << ::apply( v, []( auto n ) -> double { return n*n + 0.1; } );
}



TT(teste4) {
	auto m = vector{ vector{ 1, 3 }, { 4, 15 }, { 3, 10 } };
	cout << ::apply( m, []( auto lin ) { return lin[0]*lin[0] - lin[1]; } );
}


TT(teste5) {
	auto a = { 1, 2 }, b = { 3, 4 };
	cout << ::apply( a, [b]( auto x ) { return ::apply( b, [x]( auto y ){ return vector{ x, y }; } ); } );
}


TT(teste6) {
	cout << ::apply( vector{ 1, 2, 3 }, FunctorSimples() );
}



TT(teste7) {
	cout << ::apply( vector{ 1, 2, 3 }, FunctorTemplate() ) << ::apply( vector<string>{ "a", "b", "c" }, FunctorTemplate() );
}


TT(teste8) {
	cout << ::apply( { 1, 2, 3, 4, 5 }, roman );
}


TT(teste9) {
	int v[] = { 1, 2, 3, 4, 5 };
	vector<int> r = ::apply( v, id );
	cout << r;
}


TT(teste10) {
	int v[] = { 1, 2, 3, 4, 5 };
	vector<string> r = ::apply( v, roman );
	cout << r;
}
