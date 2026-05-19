SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++17
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8 teste9 teste10 teste11 teste12 teste13 teste14 teste15


define teste1
2 9 8 8 
jose casado 
3.11 5 7.9 
endef

define teste2
1 5 9 3 
3.1 5.3 9 
a e i o u ypsilon 
endef

define teste3
1 9 5 8 
joao Solteiro 
3.14 999 7.9 900 
endef

define teste4
[ 1, 2 ] [ 4, 6 ] [ 6, 7 ] 
1 2 4 6 6 7 
HELLO, WORLD 
endef

define teste5
1 4 9 
endef

define teste6
2
4
6
endef

define teste7
8
16
24
endef

define teste8
2 8 8 4 
endef

define teste9
2 8 8 4 
endef

define teste10
2 8 8 4 
endef

define teste11
2 8 8 4 
endef

define teste12
2.1 3.1 4.1 5.1 6.1 
endef

define teste13
B! D! F! 
endef

define teste14
F! B! D! 
endef

define teste15
7 9 6 5 4 
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
