SHELL    := /bin/bash
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra
SRC      := main.cpp
BUILD    := build


TESTS := teste1 teste2 teste3 teste4 teste5 teste6 teste7 teste8 teste9 teste10 teste11 teste12


define teste1
f(x) = ((((3)+(x)))+(x)), f'(x) = ((((0)+(1)))+(1))
endef

define teste2
f(x) = ((x)*(x)), f'(x) = ((1)*(x)+(x)*(1))
endef

define teste3
f(x) = ((x)-(((x)-(5)))), f'(x) = ((1)-(((1)-(0))))
endef

define teste4
f(x) = ((((x)+(7)))*(((x)-(7)))), f'(x) = ((((1)+(0)))*(((x)-(7)))+(((x)+(7)))*(((1)-(0))))
endef

define teste5
f(x) = (x)^3, f'(x) = (3*(1)*(x)^2)
endef

define teste6
f(x) = (((((7)*(x)))+(5)))^3, f'(x) = (3*(((((0)*(x)+(7)*(1)))+(0)))*(((((7)*(x)))+(5)))^2)
endef

define teste7
f(x) = sin(((x)+(5))), f'(x) = (cos(((x)+(5)))*((1)+(0)))
endef

define teste8
f(x) = ((1)/(((((sin(x))^2)+((cos(x))^2)))^4)), f'(x) = (((0)*(((((sin(x))^2)+((cos(x))^2)))^4)-(1)*((4*((((2*((cos(x)*1))*(sin(x))^1))+((2*((-sin(x)*1))*(cos(x))^1))))*((((sin(x))^2)+((cos(x))^2)))^3)))/(((((sin(x))^2)+((cos(x))^2)))^4)^2)
endef

define teste9
f(x) = exp(((((x)*(log(((x)-(8))))))+(1))), f'(x) = (exp(((((x)*(log(((x)-(8))))))+(1)))*((((1)*(log(((x)-(8))))+(x)*((1/(((x)-(8)))*((1)-(0))))))+(0)))
endef

define teste10
f(x) = (exp(((((x)+(1)))*(((x)-(1))))))^2, f'(x) = (2*((exp(((((x)+(1)))*(((x)-(1)))))*((((1)+(0)))*(((x)-(1)))+(((x)+(1)))*(((1)-(0))))))*(exp(((((x)+(1)))*(((x)-(1))))))^1)
endef

define teste11
f(x) = ((log((((x)^4)+(8))))*(sin(cos(((x)/(3.14)))))), f'(x) = (((1/((((x)^4)+(8)))*(((4*(1)*(x)^3))+(0))))*(sin(cos(((x)/(3.14)))))+(log((((x)^4)+(8))))*((cos(cos(((x)/(3.14))))*(-sin(((x)/(3.14)))*(((1)*(3.14)-(x)*(0))/(3.14)^2)))))
endef

define teste12
f(x) = log(((((((((((8.1)/((((x)+(1)))^3)))-(9.2)))*(((x)+(3)))))*(sin(cos(((x)/(3.14)))))))+(x))), f'(x) = (1/(((((((((((8.1)/((((x)+(1)))^3)))-(9.2)))*(((x)+(3)))))*(sin(cos(((x)/(3.14)))))))+(x)))*(((((((((((0)*((((x)+(1)))^3)-(8.1)*((3*(((1)+(0)))*(((x)+(1)))^2)))/((((x)+(1)))^3)^2))-(0)))*(((x)+(3)))+(((((8.1)/((((x)+(1)))^3)))-(9.2)))*(((1)+(0)))))*(sin(cos(((x)/(3.14)))))+(((((((8.1)/((((x)+(1)))^3)))-(9.2)))*(((x)+(3)))))*((cos(cos(((x)/(3.14))))*(-sin(((x)/(3.14)))*(((1)*(3.14)-(x)*(0))/(3.14)^2))))))+(1)))
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
