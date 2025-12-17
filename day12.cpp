#include "h/day12.h"

#include <cmath>
#include <iostream>
#include <sstream>

#include "file.h"

#define VERBOSE 1}

int day12(File& in) {
  // This solution is going to be HEAVILY optimized to my input.

  int blanks {};
  std::string line {};
  while (in.next_line(line)) {
    if (line.empty()) blanks++;
    if (blanks == 6) break;
  }

  long sol {};
  while (in.next_line(line)) {
    std::istringstream iss { line };
    long x {};
    char c {};
    long y {};
    iss >> x >> c >> y;
    std::cout << x << " * " << y;

    long piece_count {};
    int n {};
    iss.get();
    while (iss >> std::ws >> n)
      piece_count += n;

    std::cout << ": " << piece_count << " pieces\n";

    // This condition makes more sense: (piece_count <= std::floor(x / 3.0) * std::floor(y / 3.0))
    // But it turns out all I need is this:
    if (piece_count * 9 <= x * y) {
      std::cout << "It works!\n";
      sol++;
    } else {
      std::cout << "Nope\n";
    }
  }

  std::cout << sol << " of them work\n";

  return 0;
}
