#include "h/day10.h"

#include <bitset>
#include <iostream>
#include <sstream>
#include <vector>

#include "file.h"

#define VERBOSE 1

namespace Day10 {
  struct Machine;
  void get_buttons(std::istringstream& iss, Machine* machine);

  struct Machine {
    uint32_t light_seq {};
    std::vector<uint32_t> buttons {};

    explicit Machine(const std::string& line) {
      std::istringstream iss { line };

      iss.get(); // discard opening bracket

      char c {};
      int modifier {};
      while (iss.get(c)) {
        if (c == ']') break;
        if (c == '#') light_seq |= (1 << modifier);
        modifier++;
      }

      #if VERBOSE
      std::cout << "light seq " << light_seq << "\n";
      #endif

      get_buttons(iss, this);

      // TODO: Will need joltage target for part 2
    }

    [[nodiscard]] long p1_min_solve() const {
      long min { std::numeric_limits<uint32_t>::max() };

      for (uint32_t i {}; i < (1u << buttons.size()); i++) {
        std::bitset<16> bits { i }; // That should be large enough.
        uint32_t light {};
        long sum {};

        #if VERBOSE
        std::cout << bits << ": ";
        #endif

        for (int j {}; j < buttons.size(); j++) {
          if (bits.test(j)) {
            #if VERBOSE
            std::cout << buttons[j] << " ";
            #endif
            light ^= buttons[j];
            sum++;
          }
        }

        #if VERBOSE
        std::cout << "= " << light << "\n";
        #endif
        if (light == light_seq) {
          #if VERBOSE
          std::cout << "found solution with " << sum << " buttons\n";
          #endif
          min = std::min(min, sum);
          if (min <= 1) return min; // The first run will always be 0, so it'll catch it if a machine needs no buttons.
        }
      }

      return min;
    }
  };

  long p2() {
    return 0;
  }

  void get_buttons(std::istringstream& iss, Machine* machine) {
    std::string trash {};
    do {
      char c {};
      while (iss.get(c) && c != '(' && c != '{') {}
      if (c == '{') return;

      uint32_t button {};

      int button_num {};
      #if VERBOSE
      std::cout << "  button";
      #endif
      while (iss >> button_num) {
        #if VERBOSE
        std::cout << " " << button_num;
        std::cout << " [[" << (1u << button_num) << "]]";
        #endif
        button |= (1u << button_num);

        if (iss.peek() == ')') break;
        if (iss.peek() == ',') iss.get();
      }
      #if VERBOSE
      std::cout << " = " << button << "\n";
      #endif

      machine->buttons.push_back(button);
    } while (true);
  }
}

int day10(File& in) {
  using namespace Day10;

  long sol1 {};

  std::string line {};
  while (in.next_line(line)) {
    Machine machine { line };

    sol1 += machine.p1_min_solve();
  }

  std::cout << "PART 1: " << sol1 << "\n";
  // const long sol2 { p2() };
  // std::cout << "PART 2: Answer " << sol2 << "\n";

  return 0;
}
