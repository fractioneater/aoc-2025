#include "h/day10.h"

#include <iostream>

#include "file.h"

#define VERBOSE 1

long p1() {
  return 0;
}

long p2() {
  return 0;
}

int day10(File& in) {
  // PLAN: ...

  std::string line {};
  while (in.next_line(line)) {}

  const long sol1 { p1() };
  std::cout << "PART 1: Answer" << sol1 << "\n";
  // const long sol2 { p2() };
  // std::cout << "PART 2: Answer" << sol2 << "\n";

  return 0;
}
