// Copyright 2026 Onotin Maxim

#include "../lib_algoritms/algoritms.h"

#include <stdexcept>
#include <string>

#include "../lib_stack_on_mass/Stack.h"

float division(int a, int b) {
  if (b == 0) {
    throw std::invalid_argument("Input Error: can't divide by zero!");
  }
  return static_cast<float>(a) / b;
}

bool check_breckets(std::string expression) {
  StackOnMass<char> stack(expression.length());

  for (size_t i = 0; i < expression.length(); i++) {
    char sym = expression[i];

    if (sym == '(' || sym == '[' || sym == '{') {
      stack.push(sym);
    } else if (sym == ')' || sym == ']' || sym == '}') {
      if (stack.is_empty()) {
        return false;
      }

      char last = stack.pop();

      if ((sym == ')' && last != '(') || (sym == ']' && last != '[') ||
          (sym == '}' && last != '{')) {
        return false;
      }
    }
  }

  return stack.is_empty();
}