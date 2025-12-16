#include "h/day11.h"

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>

#include "file.h"

#define VERBOSE 1

namespace Day11 {
  long p1(std::map<std::string, std::map<std::string, long>> connections) {
    long sol1 {};
    std::queue<std::string> queue {};
    queue.emplace("you");

    while (!queue.empty()) {
      std::string device { queue.front() };
      queue.pop();
      if (device == "out") {
        ++sol1;
        continue;
      }

      for (const auto& key : connections[device] | std::views::keys) {
        queue.emplace(key);
      }
    }

    return sol1;
  }

  long p2(std::map<std::string, std::map<std::string, long>>& connections) {
    for (auto& [this_name, outer] : connections) {
      if (this_name == "svr" || this_name == "fft" || this_name == "dac") continue;

      #if VERBOSE
      std::cout << "collapsing " << this_name << "\n";
      #endif
      // All other devices. Values only (device name excluded, it doesn't matter).
      for (auto& [other_name, other_conn] : connections) {
        if (other_name == this_name) continue; // Don't need to check this, but I will anyway.
        if (other_conn.empty()) continue;
        #if VERBOSE
        std::cout << "  " << other_name << " gets ";
        #endif
        // All connections of the OTHER device, to test if one is THIS device (that's being removed).
        for (const auto& other_conn_name : other_conn | std::views::keys) {
          if (this_name == other_conn_name) {
            // All connections of THIS device that are substituted with just a link to this device.
            for (auto& [this_conn_name, this_conn_count] : outer) {
              if (this_conn_count == 0) continue;
              #if VERBOSE
              std::cout << this_conn_name << " (" << other_conn[this_conn_name] << "+" << this_conn_count << "), ";
              #endif
              // Add: how many links to THIS the other one has * how many this conn would give. This multiplication was the troublesome bit.
              other_conn[this_conn_name] += other_conn[this_name] * this_conn_count;
            }
            other_conn[this_name] = 0l;
          }
        }
        #if VERBOSE
        std::cout << "\n";
        #endif
      }

      outer.clear();
    }

    #if VERBOSE
    for (const auto& [name, links] : connections) {
      if (links.empty()) continue;
      std::cout << name << ": ";
      for (const auto& [other_name, count] : links) {
        if (count == 0) continue;
        std::cout << "{" << other_name << " * " << count << "} ";
      }
      std::cout << "\n";
    }
    #endif

    long sol2 { connections["svr"]["fft"] * connections["fft"]["dac"] * connections["dac"]["out"] };
    if (sol2 == 0) sol2 = connections["svr"]["dac"] * connections["dac"]["fft"] * connections["fft"]["out"];

    return sol2;
  }
}

int day11(File& in) {
  using namespace Day11;

  // PLAN: BFS (for p1). Then collapse the tree (for p2). I know I could do memoization and the same thing as part 1.

  std::map<std::string, std::map<std::string, long>> connections {};

  std::string line {};
  while (in.next_line(line)) {
    std::istringstream iss { line };
    std::string device {};
    std::getline(iss, device, ':');

    std::string connection {};
    while (std::getline(iss >> std::ws, connection, ' ')) {
      connections[device][connection] = 1;
    }
  }

  const long sol1 { p1(connections) };
  std::cout << "PART 1: Answer " << sol1 << "\n";
  const long sol2 { p2(connections) };
  std::cout << "PART 2: Answer " << sol2 << "\n";

  return 0;
}
