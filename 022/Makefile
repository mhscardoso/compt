SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -pthread -O2
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8 teste9 teste10


define teste1
OK
endef

define teste2
OK
endef

define teste3
320000;320000
endef

define teste4
200000;2100000
endef

define teste5
200000;0;0
endef

define teste6
160000;140000;280000
endef

define teste7
5000;20000
endef

define teste8
80000;80000
endef

define teste9
150000;200000
endef

define teste10
140000;220000
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
