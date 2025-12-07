#include <iostream>
#include <ranges>
#include <sstream>
#include <vector>
#include "file.h"

#define VERBOSE 0
#define PART 1

struct Splitter {
  int column {};
  int row {};
  bool is_blocked { false };
  bool is_marked { false };
  int print_indent {};
  std::vector<Splitter*> dependents {};

  void mark() {
    #if VERBOSE
    std::cout << "Marking (" << column << ", " << row << ")\n";
    #endif
    is_marked = true;
    for (Splitter* sub : dependents) {
      if (sub->is_marked) continue;
      sub->mark();
    }
  }
};

std::ostream& operator<<(std::ostream& out, const Splitter& splitter) {
  for (int indent {}; indent < splitter.print_indent; ++indent)
    std::cout << "  ";

  out << "Column: " << splitter.column << ", row: " << splitter.row << "\n";
  for (auto* dependent : splitter.dependents) {
    dependent->print_indent = splitter.print_indent + 1;
    out << *dependent;
  }
  return out;
}

int main() {
  File in;
  try {
    in = File { 7, "input" };
  } catch (int ex) { return ex; }

  // Plan: Make a tree to store which splitters depend on which higher ones. I'll start from the bottom and go up.

  std::vector<std::istringstream> lines {};
  std::string line {};
  while (in.next_line(line)) {
    lines.push_back(std::move(static_cast<std::istringstream>(line)));
  }
  std::reverse(lines.begin(), lines.end());

  std::vector<Splitter> splitters {};
  splitters.reserve(1777); // Because I don't want dangling pointers, and my input has 1776. This is stupid.

  std::string trash;
  std::getline(lines.back(), trash, 'S');
  lines.erase(lines.end() - 1);

  int start_column { static_cast<int>(trash.size()) };

  int row {};
  for (auto& l : lines) {
    std::string empty_dots {};
    int col {};
    while (std::getline(l, empty_dots, '^')) {
      col += static_cast<int>(empty_dots.size());
      if (col == l.str().size()) break;

      Splitter s { col, row };

      for (
        auto& other : std::ranges::filter_view(splitters, [col](const Splitter& it) { return col - 1 <= it.column && it.column <= col + 1; })
      ) {
        // Blocking others
        if (other.column == col) {
          #if VERBOSE
          std::cout << "(" << other.column << ", " << other.row << ") is blocked in row " << row << "\n";
          #endif

          other.is_blocked = true;
        }

        // Passing beam to others
        if (std::abs(other.column - col) == 1 && !other.is_blocked) {
          #if VERBOSE
          std::cout << "Beam can travel from (" << col << ", " << row << ") to (" << other.column << ", " << other.row << ")\n";
          #endif

          s.dependents.push_back(&other);
        }
      }
      col++; // A splitter takes up a char.
      splitters.push_back(std::move(s));
    }
    row++;
  }

  auto& tree_start {
    *std::ranges::find_if(
      splitters, [start_column](const auto& it) {
        return it.column == start_column && !it.is_blocked;
      }
    )
  };

  #if VERBOSE
  std::cout << "----\n";
  #endif

  tree_start.mark();
  int answer {
    static_cast<int>(std::count_if(splitters.begin(), splitters.end(), [](const Splitter& it) { return it.is_marked; }))
  };
  std::cout << "PART: " << PART << "\nThe beam is split " << answer << " times\n";

  #if VERBOSE
  std::cout << "----\n" << splitters.back() << '\n';
  #endif

  return 0;
}
