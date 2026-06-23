// Banca de Testes

#include <tuple>
#include <functional>
#include <iostream>

#include "lib.h"

long mdc( long a, long b ) { return b == 0 ? a : mdc( b, a%b ); }

struct MMC {
  auto operator()( long a, long b ) { return a*b/mdc(a,b); }
};

struct BarraPesada {
  template <typename A, typename B>
  auto operator()( A a, B b ) { return a + b; }
};

string ordena( string a, string b, string c, string d,
               string e, string f, string g, string h,
               string i, string j, string k, string l ) {
  std::array<string, 12> tab = { a,b,c,d,e,f,g,h,i,j,k,l };
  string result;
  std::sort( tab.begin(), tab.end() );
  for( auto itr = tab.begin(); itr != tab.end(); ++itr )
    result += *itr + " ";
  return result;
}



int main(void) {

    // using ::bind;

#ifdef teste1

    auto f2 = ::bind( mdc, __ );
    auto f1 = ::bind( f2, __, 12 );
    for( int i = 2; i <= 12; i++ )
      cout << f1( i ) <<  " ";



#elif defined(teste2)
    auto f2 = ::bind( mdc );
    auto f1 = f2( 18, __ );
    for( int i = 2; i <= 18; i++ )
      cout << f1( i ) <<  " ";



#elif defined(teste3)
    auto f2 = ::bind( []( int x, int y, int z ){ cout << x*y << (char) z << " " ; }, __, __, ',' );
    for( int i = 0; i < 5; i++ )
      f2( i, 10 );



#elif defined(teste4)
    auto f3 = ::bind( []( int x, int y, int z ){ cout << x*z << (char) y << " " ; } );
    auto f1 = f3( __, ';' );
    for( int i = 0; i < 5; i++ )
      f1( 5 )( i );



#elif defined(teste5)
    MMC mmc;
    auto f1 = ::bind( mmc, __, 6, __ );
    for( int i = 2; i <= 12; i++ )
      cout << f1( i ) << " ";



#elif defined(teste6)
    MMC mmc;
    auto f2 = ::bind( mmc );
    auto f1 = f2( __, 6 );
    for( int i = 2; i <= 12; i++ )
      cout << f1( i ) << " ";



#elif defined(teste7)
    auto f1 = ::bind( ordena, "a", "b", __, "6", __, "1", "0", "c", __, __, "e" );
    auto f2 = f1( "9", __, "2", "d", "s" );
    cout << f2( "@" ) << endl;
    cout << f2( "~" );



#elif defined(teste8)
    auto f2=::bind( BarraPesada(), __, (string) "a" );
    cout << f2( (string) "b" ) << endl;
    cout << ::bind( BarraPesada(), __, 3.1)( 4.3 );



#elif defined(teste9)
    BarraPesada barraMaisPesada;
    auto f=::bind( &BarraPesada::foo, __, 7, __ );
    cout << f( barraMaisPesada )( 3 ) << endl;
    auto ff = f( __, 5 );
    cout << ff( barraMaisPesada );


#elif defined(teste10)
    auto f1 = ::bind( []( auto... args ){ return (args + ... + 0 ); }, __, __, 10 );
    auto f3 = f1( 9, 3.14 );
    cout << f3 << endl;

#endif

  return 0;

}
