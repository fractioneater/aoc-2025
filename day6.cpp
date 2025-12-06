#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>
#include "file.h"

#define VERBOSE 1
#define PART 1

int main() {
  File in;
  try { in = File { 6, "input" }; } catch (int ex) { return ex; }

  long sum {};

  std::vector<std::istringstream> lines {};

  std::string line {};
  while (in.next_line(line)) {
    std::istringstream iss { line };
    lines.push_back(std::move(iss));
  }

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

  std::cout << "PART: " << PART << "\nThe grand total is " << sum << "\n";

  return 0;
}
