#include <fstream>
#include <iostream>
#include <sstream>

#define VERBOSE 1
#define PART 2

int main() {
  int dial {50};
  int password {};

  std::ifstream in {"../day1.input"};
  if (!in) {
    std::cerr << "Couldn't open input file.\n";
    return 1;
  }

  std::string line {};
  while (std::getline(in, line)) {
    char direction {line[0]};
    std::stringstream ss {line.substr(1, line.size() - 1)};
    int turn {};
    if (!(ss >> turn)) {
      std::cerr << "Couldn't convert to int.\n";
      return 1;
    }

    int old_dial {dial};
    if (direction == 'L') {
      dial -= turn;
    } else if (direction == 'R') {
      dial += turn;
    } else {
      std::cerr << "Invalid direction.\n";
      return 1;
    }

#   if PART == 1
    if (dial % 100 == 0) {
      ++password;
      dial = 0;
    }

#   elif PART == 2
    // It's ridiculous how long this took me.
    if (dial <= 0 && old_dial != 0) {
#     if VERBOSE
      std::cout << "Hit or passed REAL zero\n";
#     endif
      ++password;
    }
    std::cout << "Passed zero " << std::abs(dial / 100) << " times\n";
    password += std::abs(dial / 100);
    dial     %= 100;
    if (dial < 0) dial += 100;
#   endif

#   if VERBOSE
    std::cout << direction << " turn of " << turn << " [" << old_dial << " -> " << dial << "]\n\n";
#   endif
  }

  std::cout << "PART: " << PART << "\nThe password is " << password << '\n';

  return 0;
}
