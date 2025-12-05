#include <iostream>
#include <vector>
#include "file.h"

#define VERBOSE 1
#define PART 2

struct Spot {
  std::vector<Spot*> neighbors {};
  char type {};

  void add_neighbor(Spot& other) {
    if (other.type != '@') return;
    neighbors.push_back(&other);
  }

  void yoink(int& counter) {
    type = '.';
    ++counter;
    for (const auto n : neighbors) {
      if (n->type == '@') {
        auto this_one { std::ranges::find(n->neighbors, this) };
        n->neighbors.erase(this_one);
        if (n->neighbors.size() < 4) n->yoink(counter);
      }
    }
  }
};

int main() {
  File in;
  // IMPORTANT: Change size accordingly when switching between input and sample.
  //            Sample should be 10, input should be 140.
  try { in = File { 4, "input" }; } catch (int ex) { return ex; }

  // Hardcoded values; probably not good practice, but I have the input file.
  constexpr int size { 140 };

  std::array<Spot, size * size> spots {};

  // Current spot in the array.
  int i { 0 };

  std::string line {};
  int n { 0 };
  while (in.next_line(line)) {
    for (int c {}; c < size; c++) { // Heh heh, name dropping the language I'm using
      spots[i]      = Spot {};
      spots[i].type = line.at(c);

      bool top { n > 0 };
      bool left { c > 0 };
      bool right { c < size - 1 };

      // I'm so glad I made the member function add_neighbor.
      if (top) {
        spots[i].add_neighbor(spots[i - size]);
        if (left) {
          spots[i].add_neighbor(spots[i - size - 1]);
          // We fill in the missing data of the tile above and left of the current one.
          if (c > 1) {
            spots[i - size - 1].add_neighbor(spots[i - 2]); // Bottom left
          }
          spots[i - size - 1].add_neighbor(spots[i]);     // Bottom right (relative to INCOMPLETE tile)
          spots[i - size - 1].add_neighbor(spots[i - 1]); // Bottom
        }

        if (right) {
          spots[i].add_neighbor(spots[i - size + 1]);
        } else {
          // We're at the far right of a row, filling in missing data for the tile above.
          spots[i - size].add_neighbor(spots[i]);     // Bottom (relative to INCOMPLETE tile)
          spots[i - size].add_neighbor(spots[i - 1]); // Bottom left
        }
      }

      if (left) {
        spots[i].add_neighbor(spots[i - 1]);
        spots[i - 1].add_neighbor(spots[i]);
      }
      ++i;
    }
    n++;
  }

  #if PART == 1
  #if VERBOSE
  for (int tile {}; tile < spots.size(); ++tile) {
    const auto& [neighbors, type] { spots[tile] };
    if (type == '.') continue;
    if (neighbors.size() >= 4) continue;
    std::cout << "(" << tile % size << ", " << tile / size << ") crowdedness: " << neighbors.size() << '\n';
  }
  #endif

  std::cout << "PART: 1\nSum is " << std::count_if(
    spots.begin(), spots.end(), [](const Spot& s) {
      return s.type == '@' && s.neighbors.size() < 4;
    }
  ) << '\n';

  #elif PART == 2

  int removed {};
  int removed_recently { -1 };
  while (true) {
    if (removed_recently == 0) break;
    removed_recently = 0;
    auto spot {
      std::ranges::find_if(spots, [](const Spot& s) { return s.type == '@' && s.neighbors.size() < 4; })
    };
    if (spot != spots.end()) {
      spot->yoink(removed_recently);
    }

    #if VERBOSE
    std::cout << removed_recently << " were/was removed in this cycle.\n";
    #endif
    removed += removed_recently;
  }
  std::cout << "Part: 2\n" << removed << " rolls can be removed\n";

  #endif
  return 0;
}

/* Comments and stuff:
 * Yeah, I'm not doing this anymore. (Documenting my experience and growth as a C++ learner; of course I'll still solve the puzzles.)
 */
