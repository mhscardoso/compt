WORK=001
JUDGE=0

all:main.cpp
	g++ -std=c++17 -D WORK=$(WORK) -D JUDGE=$(JUDGE) -Wall main.cpp -o main && \
	./main
