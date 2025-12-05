#include <iostream>
#include <sstream>
#include "file.h"

#define VERBOSE 0
#define PART 2

int main() {
  int dial { 50 };
  int password {};

  File in;
  try { in = File { 1, "input" }; } catch (int ex) { return ex; }

  std::string line {};
  while (in.next_line(line)) {
    const char direction { line[0] };
    std::stringstream ss { line.substr(1, line.size() - 1) };
    int turn {};
    if (!(ss >> turn)) {
      std::cerr << "Couldn't convert to int.\n";
      return 1;
    }

    const int old_dial { dial };
    if (direction == 'L') {
      dial -= turn;
    } else if (direction == 'R') {
      dial += turn;
    } else {
      std::cerr << "Invalid direction.\n";
      return 1;
    }

    #if PART == 1
    if (dial % 100 == 0) {
      ++password;
      dial = 0;
    }

    #elif PART == 2
    if (dial <= 0 && old_dial != 0) {
      #if VERBOSE
      std::cout << "Hit or passed REAL zero\n";
      #endif

      ++password;
    }

    #if VERBOSE
    std::cout << "Passed zero " << std::abs(dial / 100) << " times\n";
    #endif

    password += std::abs(dial / 100);
    dial     %= 100;
    if (dial < 0) dial += 100;
    #endif

    #if VERBOSE
    std::cout << direction << " turn of " << turn << " [" << old_dial << " -> " << dial << "]\n\n";
    #endif
  }

  std::cout << "PART: " << PART << "\nThe password is " << password << '\n';

  return 0;
}

/* Comments and stuff:
 * Part 1:
 *   Simple. I'm sure there are more interesting ways to do it, but this is fine.
 * Part 2:
 *   It's ridiculous how many attempts this took me. I didn't plan before writing it, and I didn't thoroughly
 *   test afterward.
 */
