WORK=001
JUDGE=0

all:main.cpp
	@echo "Testes de $(WORK) -> $(JUDGE)"
	@echo ""
	g++ -std=c++20 -D WORK=$(WORK) -D JUDGE=$(JUDGE) -Wall main.cpp -o main && \
	./main
