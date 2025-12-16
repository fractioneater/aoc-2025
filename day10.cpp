#include "h/day10.h"

#include <bitset>
#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>

#include "file.h"
#include "memoizer.cpp"

#define VERBOSE 2

namespace Day10 {
  struct Machine {
    int line_num {};
    uint32_t light_seq {};
    std::vector<uint32_t> buttons {};
    std::vector<int> goal {};

    std::map<std::vector<int>, std::map<std::vector<int>, int>> pattern_costs;

    void get_light_seq(std::istringstream& iss) {
      iss.get(); // discard opening bracket

      char c {};
      int modifier {};
      while (iss.get(c)) {
        if (c == ']') break;
        if (c == '#') light_seq |= (1 << modifier);
        modifier++;
      }

      #if VERBOSE == 1
      std::cout << "light seq " << light_seq << "\n";
      #endif
    }

    void get_buttons(std::istringstream& iss) {
      std::string trash {};
      do {
        char c {};
        while (iss.get(c) && c != '(' && c != '{') {}
        if (c == '{') return;

        uint32_t button {};

        int button_num {};
        #if VERBOSE == 1
        std::cout << "  button";
        #endif
        while (iss >> button_num) {
          #if VERBOSE == 1
          std::cout << " " << button_num;
          std::cout << " [[" << (1u << button_num) << "]]";
          #endif
          button |= (1u << button_num);

          if (iss.peek() == ')') break;
          if (iss.peek() == ',') iss.get();
        }
        #if VERBOSE == 1
        std::cout << " = " << button << "\n";
        #endif

        buttons.push_back(button);
      } while (true);
    }

    void get_goal(std::istringstream& iss) {
      int n {};
      while (iss >> n) {
        goal.push_back(n);

        if (iss.peek() == '}') break;
        if (iss.peek() == ',') iss.get();
      }

      #if VERBOSE == 1
      std::cout << "goal";
      for (const auto a : goal) {
        std::cout << " " << a;
      }
      std::cout << "\n";
      #endif
    }

    explicit Machine(int n, const std::string& line) : line_num { n } {
      std::istringstream iss { line };
      get_light_seq(iss);
      get_buttons(iss);
      get_goal(iss);
    }

    [[nodiscard]] long p1() const {
      long min { std::numeric_limits<uint32_t>::max() };

      for (uint32_t i {}; i < (1u << buttons.size()); i++) {
        std::bitset<16> bits { i }; // That should be large enough.
        uint32_t light {};

        #if VERBOSE == 1
        std::cout << bits << ": ";
        #endif

        for (int j {}; j < buttons.size(); j++) {
          if (bits.test(j)) {
            #if VERBOSE == 1
            std::cout << buttons[j] << " ";
            #endif
            light ^= buttons[j];
          }
        }

        #if VERBOSE == 1
        std::cout << "= " << light << "\n";
        #endif
        if (light == light_seq) {
          long pressed_count { std::popcount(i) };
          #if VERBOSE == 1
          std::cout << "found solution with " << pressed_count << " buttons\n";
          #endif
          min = std::min(min, pressed_count);
          if (min <= 1) return min; // The first run will always be 0, so it'll catch it if a machine needs no buttons.
        }
      }

      return min;
    }

    [[nodiscard]] auto patterns() {
      // Iterate through all button combinations (0 to 1u << buttons.size())
      for (uint32_t b {}; b < (1u << buttons.size()); b++) {
        std::bitset<16> bits { b };
        std::vector pattern(goal.size(), 0);

        // XOR the buttons together to get the pattern
        for (int j {}; j < buttons.size(); j++) {
          if (bits.test(j)) {
            for (int index {}; index < goal.size(); index++) {
              if (buttons[j] & (1 << index)) pattern[index]++;
            }
          }
        }

        // Extract the parity
        std::vector<int> parity(goal.size());
        for (int i = 0; i < goal.size(); i++) {
          parity[i] = pattern[i] % 2;
        }

        // Only store if we haven't seen this pattern before
        if (!pattern_costs[parity].contains(pattern) || pattern_costs[parity][pattern] > std::popcount(b))
          pattern_costs[parity][pattern] = std::popcount(b);
      }
    }

    // Thank you so much to the person on Reddit who found this solution!
    long p2() {
      patterns();

      Memoizer<int, std::vector<int>>* solve_p2_aux_memo_heh_heh_long_variable_name;

      const std::function<int(std::vector<int>)> solve_p2_aux {
        [this, &solve_p2_aux_memo_heh_heh_long_variable_name](std::vector<int> rec_goal) {
          if (std::all_of(rec_goal.begin(), rec_goal.end(), [](int it) { return it == 0; })) return 0;
          int answer { 100000000 };

          std::vector<int> parity(rec_goal.size());
          for (int i {}; i < rec_goal.size(); i++)
            parity[i] = rec_goal[i] & 1;

          auto& matching_patterns = pattern_costs[parity];

          for (auto& [pattern, pattern_cost] : matching_patterns) {
            bool valid = true;
            for (int i {}; i < rec_goal.size(); i++)
              if (pattern[i] > rec_goal[i]) {
                valid = false;
                break;
              }
            if (!valid) continue;

            // Find new goal
            std::vector<int> new_goal(rec_goal.size());
            for (int i {}; i < rec_goal.size(); i++) {
              new_goal[i] = (rec_goal[i] - pattern[i]) / 2;
            }

            answer = std::min(answer, pattern_cost + 2 * (*solve_p2_aux_memo_heh_heh_long_variable_name)(new_goal));
          }
          return answer;
        }
      };

      auto memo_p2 { Memoizer { solve_p2_aux } };
      solve_p2_aux_memo_heh_heh_long_variable_name = &memo_p2;
      const int sol { memo_p2(goal) };

      #if VERBOSE
      std::cout << "Machine " << line_num << ": " << sol << " presses\n";
      #endif

      return sol;
    }
  };
}

int day10(File& in) {
  using namespace Day10;

  long sol1 {};
  long sol2 {};

  std::string line {};
  int n {};
  while (in.next_line(line)) {
    Machine machine { ++n, line };

    sol1 += machine.p1();
    sol2 += machine.p2();
  }

  std::cout << "PART 1: Minimum " << sol1 << " presses\n";
  std::cout << "PART 2: Minimum " << sol2 << " presses\n";

  return 0;
}
