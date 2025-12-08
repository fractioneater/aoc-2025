#include "h/day6.h"

#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>

#include "file.h"

#define VERBOSE 1
#define PART 2

int column_to_int(std::vector<std::istringstream>& lines) {
  char col[lines.size()];
  for (int r {}; r < lines.size() - 1; ++r) {
    lines[r].get(col[r]);
  }
  col[lines.size() - 1] = '\0';

  int n {};
  try {
    n = std::stoi(col);
  } catch (const std::invalid_argument&) {
    return -1;
  }

  return n;
}

long solve_a_problem(std::vector<std::istringstream>& lines) {
  char op;
  lines.back() >> op;

  long result { op == '*' ? 1 : 0 }; // Identity

  int x { column_to_int(lines) };
  do {
    if (op == '*') result *= x;
    else result += x;
    x = column_to_int(lines);
  } while (x != -1);

  return result;
}

int day6(File& in) {
  long sum {};

  std::vector<std::istringstream> lines {};

  std::string line {};
  while (in.next_line(line)) {
    std::istringstream iss { line };
    lines.push_back(std::move(iss));
  }

  #if PART == 1
  while (true) {
    char op { static_cast<char>((lines[lines.size() - 1] >> std::ws).get()) };
    if (op == -1) break;

    std::vector<std::string> num_strings {};
    for (int i {}; i < lines.size() - 1; ++i) {
      num_strings.resize(num_strings.size() + 1);
      std::getline(lines[i] >> std::ws, num_strings[i], ' ');

      #if VERBOSE
      std::cout << num_strings[i];
      if (i == lines.size() - 2) std::cout << '\n';
      else std::cout << " " << op << " ";
      #endif
    }

    std::vector<int> nums(num_strings.size());
    std::transform(
      num_strings.begin(), num_strings.end(), nums.begin(), [](const std::string& str) {
        return std::stoi(str);
      }
    );

    switch (op) {
      case '+':
        sum += std::accumulate(nums.begin(), nums.end(), 0l);
        break;
      case '*':
        sum += std::accumulate(nums.begin(), nums.end(), 1l, std::multiplies());
        break;
      default: ;
    }
  }

  #elif PART ==2
  while (true) {
    const long solution { solve_a_problem(lines) };
    if (solution == -1) break;
    sum += solution;
  }

  #endif

  std::cout << "PART: " << PART << "\nThe grand total is " << sum << "\n";

  return 0;
}
