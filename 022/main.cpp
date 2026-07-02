// Banca de Testes

#include <iostream>
#include <vector>
#include <thread>
#include <atomic>

#include "lib.h"

int main(void) {

#ifdef teste1
VetorSeguro v;

const int nthreads = 16;
atomic<int> prontas = 0;
atomic<bool> largada = false;
atomic<bool> ok = true;

vector<thread> threads;

for( int i = 0; i < nthreads; ++i ) {
  threads.emplace_back( [&]() {
    ++prontas;

    while( !largada )
      this_thread::yield();

    for( int j = 0; j < 10000; ++j ) {
      if( v.tamanho() != 0 || v.soma() != 0 )
        ok = false;
    }
  });
}

while( prontas < nthreads )
  this_thread::yield();

largada = true;

for( auto& t : threads )
  t.join();

cout << ( ok ? "OK" : "ERRO" );


#elif defined(teste2)
VetorSeguro v(1000, 3);

const int nthreads = 16;
atomic<int> prontas = 0;
atomic<bool> largada = false;
atomic<bool> ok = true;

vector<thread> threads;

for( int i = 0; i < nthreads; ++i ) {
  threads.emplace_back( [&]() {
    ++prontas;

    while( !largada )
      this_thread::yield();

    for( int j = 0; j < 10000; ++j ) {
      if( v.tamanho() != 1000 || v.soma() != 3000 )
        ok = false;
    }
  });
}

while( prontas < nthreads )
  this_thread::yield();

largada = true;

for( auto& t : threads )
  t.join();

cout << ( ok ? "OK" : "ERRO" );


#elif defined(teste3)
VetorSeguro v;

const int nthreads = 16;
const int repeticoes = 20000;

atomic<int> prontas = 0;
atomic<bool> largada = false;

vector<thread> threads;

for( int i = 0; i < nthreads; ++i ) {
  threads.emplace_back( [&]() {
    ++prontas;

    while( !largada )
      this_thread::yield();

    for( int j = 0; j < repeticoes; ++j )
      v.inserir(1);
  });
}

while( prontas < nthreads )
  this_thread::yield();

largada = true;

for( auto& t : threads )
  t.join();

cout << v.tamanho() << ";" << v.soma();


#elif defined(teste4)
VetorSeguro v;

const int nthreads = 20;

atomic<int> prontas = 0;
atomic<bool> largada = false;

vector<thread> threads;

for( int i = 0; i < nthreads; ++i ) {
  threads.emplace_back( [&v, &prontas, &largada, i]() {
    ++prontas;

    while( !largada )
      this_thread::yield();

    for( int j = 0; j < 10000; ++j )
      v.inserir(i + 1);
  });
}

while( prontas < nthreads )
  this_thread::yield();

largada = true;

for( auto& t : threads )
  t.join();

cout << v.tamanho() << ";" << v.soma();


#elif defined(teste5)
VetorSeguro v(200000, 1);

const int nthreads = 20;
const int tentativas = 20000;

atomic<int> prontas = 0;
atomic<bool> largada = false;
atomic<int> removidos = 0;

vector<thread> threads;

for( int i = 0; i < nthreads; ++i ) {
  threads.emplace_back( [&]() {
    ++prontas;

    while( !largada )
      this_thread::yield();

    for( int j = 0; j < tentativas; ++j ) {
      if( v.removerUltimo() )
        ++removidos;
    }
  });
}

while( prontas < nthreads )
  this_thread::yield();

largada = true;

for( auto& t : threads )
  t.join();

cout << removidos << ";" << v.tamanho() << ";" << v.soma();


#elif defined(teste6)
VetorSeguro v(300000, 2);

const int nthreads = 16;
const int tentativas = 10000;

atomic<int> prontas = 0;
atomic<bool> largada = false;
atomic<int> removidos = 0;

vector<thread> threads;

for( int i = 0; i < nthreads; ++i ) {
  threads.emplace_back( [&]() {
    ++prontas;

    while( !largada )
      this_thread::yield();

    for( int j = 0; j < tentativas; ++j ) {
      if( v.removerUltimo() )
        ++removidos;
    }
  });
}

while( prontas < nthreads )
  this_thread::yield();

largada = true;

for( auto& t : threads )
  t.join();

cout << removidos << ";" << v.tamanho() << ";" << v.soma();


#elif defined(teste7)
VetorSeguro v;

const int nthreads = 16;

atomic<int> prontas = 0;
atomic<bool> largada = false;

vector<thread> threads;

for( int i = 0; i < nthreads; ++i ) {
  threads.emplace_back( [&]() {
    ++prontas;

    while( !largada )
      this_thread::yield();

    for( int j = 0; j < 1000; ++j )
      v.redimensionar(5000, 4);
  });
}

while( prontas < nthreads )
  this_thread::yield();

largada = true;

for( auto& t : threads )
  t.join();

cout << v.tamanho() << ";" << v.soma();


#elif defined(teste8)
VetorSeguro v;

{
  const int nthreads = 16;
  const int repeticoes = 10000;

  vector<thread> threads;

  for( int i = 0; i < nthreads; ++i ) {
    threads.emplace_back( [&]() {
      for( int j = 0; j < repeticoes; ++j )
        v.inserir(1);
    });
  }

  for( auto& t : threads )
    t.join();
}

{
  const int nthreads = 16;

  atomic<int> prontas = 0;
  atomic<bool> largada = false;

  vector<thread> threads;

  for( int i = 0; i < nthreads; ++i ) {
    threads.emplace_back( [&]() {
      ++prontas;

      while( !largada )
        this_thread::yield();

      for( int j = 0; j < 500; ++j )
        v.redimensionar(80000, 9);
    });
  }

  while( prontas < nthreads )
    this_thread::yield();

  largada = true;

  for( auto& t : threads )
    t.join();
}

cout << v.tamanho() << ";" << v.soma();


#elif defined(teste9)
VetorSeguro v;

{
  const int nthreads = 10;
  const int repeticoes = 10000;

  vector<thread> threads;

  for( int i = 0; i < nthreads; ++i ) {
    threads.emplace_back( [&]() {
      for( int j = 0; j < repeticoes; ++j )
        v.inserir(1);
    });
  }

  for( auto& t : threads )
    t.join();
}

{
  const int nthreads = 16;

  atomic<int> prontas = 0;
  atomic<bool> largada = false;

  vector<thread> threads;

  for( int i = 0; i < nthreads; ++i ) {
    threads.emplace_back( [&]() {
      ++prontas;

      while( !largada )
        this_thread::yield();

      for( int j = 0; j < 500; ++j )
        v.redimensionar(150000, 2);
    });
  }

  while( prontas < nthreads )
    this_thread::yield();

  largada = true;

  for( auto& t : threads )
    t.join();
}

cout << v.tamanho() << ";" << v.soma();


#elif defined(teste10)
VetorSeguro v;

{
  const int nthreads = 20;
  const int repeticoes = 5000;

  vector<thread> threads;

  for( int i = 0; i < nthreads; ++i ) {
    threads.emplace_back( [&]() {
      for( int j = 0; j < repeticoes; ++j )
        v.inserir(1);
    });
  }

  for( auto& t : threads )
    t.join();
}

{
  const int nthreads = 16;

  vector<thread> threads;

  for( int i = 0; i < nthreads; ++i ) {
    threads.emplace_back( [&]() {
      for( int j = 0; j < 300; ++j )
        v.redimensionar(120000, 2);
    });
  }

  for( auto& t : threads )
    t.join();
}

{
  const int nthreads = 10;
  const int tentativas = 2000;

  vector<thread> threads;

  for( int i = 0; i < nthreads; ++i ) {
    threads.emplace_back( [&]() {
      for( int j = 0; j < tentativas; ++j )
        v.removerUltimo();
    });
  }

  for( auto& t : threads )
    t.join();
}

{
  const int nthreads = 8;
  const int repeticoes = 5000;

  vector<thread> threads;

  for( int i = 0; i < nthreads; ++i ) {
    threads.emplace_back( [&]() {
      for( int j = 0; j < repeticoes; ++j )
        v.inserir(3);
    });
  }

  for( auto& t : threads )
    t.join();
}

cout << v.tamanho() << ";" << v.soma();

#endif

}
