SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4


define teste1
1 2 3 7 1 3 7 
endef

define teste2
1 2 3 1 3 
endef

define teste3
1 2 3 1 3 
endef

define teste4
1 2 3 7 1 3 7 
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
