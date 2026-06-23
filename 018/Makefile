SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8 teste9 teste10 


define teste1
1;3;5;7;9;11;
endef

define teste2
9 10 11 
endef

define teste3
10 12 14 16 18 
endef

define teste4
0 2 4 6 8 10 
endef

define teste5
0 0.6 1.2 1.8 2.4 
endef

define teste6
0;0.6;1.2;1.8;2.4;3;3.6;4.2;4.8;5.4;
endef

define teste7
1!;2!;3!;4!;5!;6!;
endef

define teste8
20;40;60;80;100;120;140;160;180;200;
endef

define teste9
a!;e!;i!;
endef

define teste10
10 12 
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
