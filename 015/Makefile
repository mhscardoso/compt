SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8


define teste1
undefined 10
3.14 uma string
endef

define teste2
Erro fatal: Essa variável não é um objeto
endef

define teste3
Erro fatal: Essa variável não pode ser usada como função
endef

define teste4
undefined,undefined,undefined,undefined,undefined,true,false,undefined,undefined
288,undefined,Xpalavra ,undefined,undefined,undefined,undefined,true,true
202,302.1,undefined,1,2.2,undefined,undefined,true,undefined
202.1,302.2,undefined,1.42857,2.31,undefined,undefined,true,undefined
undefined,undefined,abracadabrapalavra ,undefined,undefined,undefined,undefined,undefined,true
endef

define teste5
{ nome: José Maria, idade: Erro fatal: Essa variável não pode ser usada como função
object José Maria 1998
endef

define teste6
'"20.2 obaoba XX undefined"
endef

define teste7
{ nome: José Maria, idade: 29, nascimento: 1990, print: function, atr: undefined }
{ nome: José Maria, idade: 18, nascimento: 2001, print: function, atr: undefined }
{ nome: José Maria, idade: 18, nascimento: 2001, print: function, atr: undefined }
endef

define teste8
{ nome: Manoel, idade: 32, nascimento: 1987, print: function, atr: object }
{ nome: Manoel, idade: 32, nascimento: 1987, print: function, atr: undefined }
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
