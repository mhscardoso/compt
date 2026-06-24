SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8


define teste1
Valor=undefined:undefined,0,false,false
Valor=true:true,1,true,true
Valor=10:10,1,true,true
Valor=2:2,1,true,true
Valor=0:0,0,false,true
Valor=0:0,1,true,true
Valor=X:X,1,true,false
Valor=object:object,1,true,false
Valor=:,0,false,true
Valor=palavra:palavra,1,true,false
Valor= 0 : 0 ,1,true,true
endef

define teste2
0;1;4;9;16;25;36;49;64;81;
endef

define teste3
Erro fatal: Essa variável não é um objeto
endef

define teste4
25
endef

define teste5
undefined,undefined,10,5.1,saida
endef

define teste6
1!2!3!4!5!
endef

define teste7
0 4 16 36 64 
endef

define teste8
true
-1
11
endef


.PHONY: test clean $(TESTS)

test: $(TESTS)
	@echo "==============================="
	@echo "       All tests done!"
	@echo "==============================="


$(TESTS): %:
	$(shell mkdir -p $(BUILD))
	$(file > $(BUILD)/$*_expected.txt,$($*))
	@$(CXX) $(CXXFLAGS) -D$* -o $(BUILD)/$* $(SRC) 2>&1 || \
		(printf "\033[0;31m[COMPILE ERROR]\033[0m $*\n" && exit 1)
	@if diff \
		<(perl -pe 'chomp if eof' $(BUILD)/$*_expected.txt) \
		<($(BUILD)/$* | perl -pe 'chomp if eof') \
		> /dev/null 2>&1; then \
		printf "\033[0;32m[PASS]\033[0m $*\n"; \
	else \
		printf "\033[0;31m[FAIL]\033[0m $*\n"; \
		printf "  Expected:\n"; \
		cat $(BUILD)/$*_expected.txt | sed 's/^/    /'; \
		printf "  Got:\n"; \
		$(BUILD)/$* | sed 's/^/    /'; \
	fi

clean:
	@rm -rf $(BUILD)
	@echo "Build folder removed."
