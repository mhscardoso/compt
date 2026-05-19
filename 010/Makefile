SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++17
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8 teste9 teste10


define teste1
f(5.1)=78.03, f'(5.1)=30.6
endef

define teste2
f(1.1)=12.111, f'(1.1)=22.23
endef

define teste3
f(0.5)=-0.208714, f'(0.5)=-1.23783
endef

define teste4
f(3.14)=-0.53995, f'(3.14)=-2.66732
endef

define teste5
f(3.14159)=-2.65359e-06, f'(3.14159)=1
endef

define teste6
f(-7.3)=-335.727, f'(-7.3)=145.27
endef

define teste7
f(-0.5)=1, f'(-0.5)=0
endef

define teste8
f(0.1)=0.787881, f'(0.1)=-0.97467
endef

define teste9
f(3)=27, f'(3)=56.6625
endef

define teste10

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
