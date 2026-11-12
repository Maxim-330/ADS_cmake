// Copyright 2026 Maksim Onotin

#define EASY_EXAMPLE
#ifdef EASY_EXAMPLE

#include <iomanip>
#include <iostream>

#include "../lib_algoritms/algoritms.h"

int main() {
  int a, b;
  float result;

  a = 1;
  b = 4;

  try {
    result = division(a, b);
    std::cout << a << " / " << b << " = " << std::setprecision(2) << result
              << std::endl;
  } catch (std::exception& err) {
    std::cerr << err.what() << std::endl;
  }

  a = 1;
  b = 0;

  try {
    result = division(a, b);
    std::cout << a << " / " << b << " = " << std::setprecision(2) << result
              << std::endl;
  } catch (std::exception& err) {
    std::cerr << err.what() << std::endl;
  }

  return 0;
}

#endif  // EASY_EXAMPLE