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

    auto a { std::max_element(line.begin(), line.end() - 11) }; // 1
    auto b { std::max_element(a + 1, line.end() - 10) };        // 2
    auto c { std::max_element(b + 1, line.end() - 9) };         // 3
    auto d { std::max_element(c + 1, line.end() - 8) };         // 4
    auto e { std::max_element(d + 1, line.end() - 7) };         // 5
    auto f { std::max_element(e + 1, line.end() - 6) };         // 6
    auto g { std::max_element(f + 1, line.end() - 5) };         // 7
    auto h { std::max_element(g + 1, line.end() - 4) };         // 8
    auto i { std::max_element(h + 1, line.end() - 3) };         // 9
    auto j { std::max_element(i + 1, line.end() - 2) };         // 10
    auto k { std::max_element(j + 1, line.end() - 1) };         // 11
    auto l { std::max_element(k + 1, line.end()) };             // 12

#   if VERBOSE
    std::cout << *a << *b << *c << *d << *e << *f << *g << *h << *i << *j << *k << *l << '\n';
#   endif
    sum += std::stol(std::format("{}{}{}{}{}{}{}{}{}{}{}{}", *a, *b, *c, *d, *e, *f, *g, *h, *i, *j, *k, *l));

#   endif
  }

  std::cout << "PART: " << PART << "\nThe joltage sum is " << sum << '\n';

  return 0;
}
