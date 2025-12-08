#include "h/day2.h"

#include <cmath>
#include <iostream>
#include <vector>

#include "file.h"

#define VERBOSE 0
#define PART 2

#if PART == 2
bool multi_repeat(const std::string& str) {
  const auto len { str.size() };
  std::vector<int> factors {};

  for (int j { 2 }; j <= len; ++j)
    if (len % j == 0) factors.push_back(j);

  for (const int f : factors) {
    const int chunk_size { static_cast<int>(len / f) };
    #if VERBOSE
    std::cout << "Testing " << chunk_size << "x" << f << " on " << str << '\n';
    #endif

    /* Summary of all this:
     * 1. Go through the chunks (skipping the first one), if any are DIFFERENT from the first, break
     * 2. If the loop made it to the end, we know it does repeat
     * 3. Leave the loop (return) to avoid double counting
     */

    int chunk { 1 };
    for (; chunk < f; ++chunk) {
      const int start { chunk * chunk_size };
      if (str.compare(0, chunk_size, str, start, chunk_size) != 0) {
        #if VERBOSE
        std::cout << str.substr(start, chunk_size) << " doesn't match " << str.substr(0, chunk_size) << '\n';
        #endif
        break;
      }
    }
    // If we made it all the way through the loop:
    if (chunk == f) { return true; }
  }

  return false;
}
#endif

int day2(File& in) {
  long invalid_sum { 0 };

  std::string line;
  while (in.until_char(line, ',')) {
    auto pos = line.find('-');
    if (pos == std::string::npos) return 1;

    long min = std::stol(line.substr(0, pos));
    long max = std::stol(line.substr(pos + 1));

    #if VERBOSE
    std::cout << '\n' << min << " to " << max << '\n';
    #endif

    for (long i { min }; i <= max; ++i) {
      std::string str = std::to_string(i);

      #if PART == 1
      if (str.size() & 1) continue;
      size_t midpt { str.size() / 2 };
      const bool repeats { str.substr(0, midpt) == str.substr(midpt) };

      #elif PART == 2
      const bool repeats { multi_repeat(str) };
      #endif

      if (repeats) {
        #if VERBOSE
        std::cout << "**** FOUND: " << str << " repeats!\n";
        #endif
        invalid_sum += i;
      }
    }
  }

  std::cout << "PART: " << PART << "\nThe sum is " << invalid_sum << '\n';

  return 0;
}

/* Comments and stuff:
 * Part 1:
 *   Feels like brute force. It's optimized, I guess, but still not a very creative solution. The thing is,
 *   in C++ I can write inefficient code, and it will still execute without making me wait.
 * Part 2:
 *   I got it first try! This is the benefit of planning out a solution.
 */
