#include <iostream>

#include "file.h"

#define VERBOSE 1
#define PART 2

int main() {
  File in;
  try { in = File { 3, "input" }; } catch (int ex) { return ex; }

  long sum { 0 };

  std::string line {};
  while (in.next_line(line)) {
#   if PART == 1

    auto a { std::max_element(line.begin(), line.end() - 1) };
    auto b { std::max_element(line.begin() + 1 + std::distance(line.begin(), a), line.end()) };

#   if VERBOSE
    std::cout << *a << *b << '\n';
#   endif
    sum += std::stol(std::format("{}{}", *a, *b));

#   elif PART == 2

    auto a { std::max_element(line.begin(), line.end() - 11) };                                      // 1
    auto b { std::max_element(line.begin() + 1 + std::distance(line.begin(), a), line.end() - 10) }; // 2
    auto c { std::max_element(line.begin() + 1 + std::distance(line.begin(), b), line.end() - 9) };  // 3
    auto d { std::max_element(line.begin() + 1 + std::distance(line.begin(), c), line.end() - 8) };  // 4
    auto e { std::max_element(line.begin() + 1 + std::distance(line.begin(), d), line.end() - 7) };  // 5
    auto f { std::max_element(line.begin() + 1 + std::distance(line.begin(), e), line.end() - 6) };  // 6
    auto g { std::max_element(line.begin() + 1 + std::distance(line.begin(), f), line.end() - 5) };  // 7
    auto h { std::max_element(line.begin() + 1 + std::distance(line.begin(), g), line.end() - 4) };  // 8
    auto i { std::max_element(line.begin() + 1 + std::distance(line.begin(), h), line.end() - 3) };  // 9
    auto j { std::max_element(line.begin() + 1 + std::distance(line.begin(), i), line.end() - 2) };  // 10
    auto k { std::max_element(line.begin() + 1 + std::distance(line.begin(), j), line.end() - 1) };  // 11
    auto l { std::max_element(line.begin() + 1 + std::distance(line.begin(), k), line.end()) };      // 12

#   if VERBOSE
    std::cout << *a << *b << *c << *d << *e << *f << *g << *h << *i << *j << *k << *l << '\n';
#   endif
    sum += std::stol(std::format("{}{}{}{}{}{}{}{}{}{}{}{}", *a, *b, *c, *d, *e, *f, *g, *h, *i, *j, *k, *l));

#   endif
  }

  std::cout << "PART: " << PART << "\nThe joltage sum is " << sum << '\n';

  return 0;
}
