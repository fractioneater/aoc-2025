#include "h/day8.h"

#include <iostream>

#include "file.h"

#define VERBOSE 1
#define PART 1

int day8(File& in) {
  // PLAN: ...

  std::string line {};
  while (in.next_line(line)) { std::cout << line << '\n'; }

  std::cout << "PART: " << PART << "\n" << /* ... */ "\n";

  return 0;
}
