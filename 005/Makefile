SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++17
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8 teste9 teste10


define teste1
[ 1 2 3 4 5 ]
endef

define teste2
[ 0.841471 0.909297 0.14112 -0.756802 -0.958924 ]
endef

define teste3
[ 1.1 4.1 9.1 16.1 25.1 ]
endef

define teste4
[ -2 1 -1 ]
endef

define teste5
[ [ [ 1 3 ] [ 1 4 ] ] [ [ 2 3 ] [ 2 4 ] ] ]
endef

define teste6
[ I! II! III! ]
endef

define teste7
[ 2 4 6 ][ aa bb cc ]
endef

define teste8
[ I II III IV V ]
endef

define teste9
[ 1 2 3 4 5 ]
endef

define teste10
[ I II III IV V ]
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
