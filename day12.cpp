#include "h/day12.h"

#include <iostream>

#include "file.h"

#define VERBOSE 1

namespace Day12 {
  long p1() {
    return 0;
  }

  long p2() {
    return 0;
  }
}

int day12(File& in) {
  using namespace Day12;

  // PLAN: ...
    
  std::string line {};
  while (in.next_line(line)) {}

  const long sol1 { p1() };
  std::cout << "PART 1: Answer " << sol1 << "\n";
  // const long sol2 { p2() };
  // std::cout << "PART 2: Answer " << sol2 << "\n";

  return 0;
}