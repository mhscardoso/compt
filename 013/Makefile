SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8 teste9 teste10 teste11


define teste1
2 3 4 1 6 1 4 3 2 1 12 
endef

define teste2
2 3 4 1 6 1 4 3 2 1 12 
endef

define teste3
2 3 2 1 6 1 2 9 2 1 6 1 2 3 2 1 18 
endef

define teste4
0, 10, 20, 30, 40, 
endef

define teste5
0; 5; 10; 15; 20; 
endef

define teste6
6 6 12 30 6 42 24 18 30 66 12 
endef

define teste7
6 6 12 30 6 42 24 18 30 66 12 
endef

define teste8
0 1 2 6 9 @ a b c d e s 
0 1 2 6 9 a b c d e s ~ 
endef

define teste9
ab
7.4
endef

define teste10
Hello, world!
7.44
endef

define teste11
u
um
uma
uma 
uma f
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
