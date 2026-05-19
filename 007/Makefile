SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++17
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5


define teste1
jan = 1
pi = 3.14
endef

define teste2
jan = 1
2 = fev
pi = 3.14
endef

define teste3
jan = 1
2 = fev
lista = [ 1 2 3 ]
1 = 2
1.1 = 2.2
endef

define teste4
jan = 1
pi = 3.14
1 = 2
endef

define teste5
Inicio
jan = 1
2 = fev
lista = [ 1 2 3 ]
1 = 2
1.1 = 2.2
Fim
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
	@if diff $(BUILD)/$*_expected.txt <($(BUILD)/$*) > /dev/null 2>&1; then \
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
