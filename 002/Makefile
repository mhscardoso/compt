SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++17
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8


define teste1
89
13
[ 1, 3, 9 ]
{[ 1, 3, 9, 19, 18, 17, 30 ]}
endef

define teste2
5
15
endef

define teste3
[ 3, 5, 13, 19 ]
[ 3, 5, 13, 32, 9, 19 ]
endef

define teste4
[ 1, 2, 3 ]
[ 8, 3, 1, 4, 5 ]
[ 8, 3, 1, 4, 7 ]
endef

define teste5
500000, 7, 7
endef

define teste6
[ 8, 3, 1, 4, 5 ]
endef

define teste7
81
11
6
[ 5, 6, 3, 2, 9, 13 ]
3
[ 5, 6, 3 ]
endef

define teste8
3 3 3 6 6 6 12 12 12 12 12 12 24 24 24 24 24 24 24 24 
[ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 ]
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
