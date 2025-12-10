#include "h/day9.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "file.h"

#define VERBOSE 0

// Part 1 seems to be reused from yesterday.

namespace Day9 {
  struct Tile {
    long x {}, y {};
  };

  long area(const Tile& a, const Tile& b) {
    return (std::abs(a.x - b.x) + 1) * (std::abs(a.y - b.y) + 1);
  }

  struct Rect {
    int idx_a, idx_b;
    long area {};

    Rect(int a, int b, long area) : idx_a { a }, idx_b { b }, area { area } {}

    Rect& operator=(const Rect& copy) {
      if (&copy == this) return *this;

      idx_a = copy.idx_a;
      idx_b = copy.idx_b;
      area  = copy.area;

      return *this;
    }
  };

  std::ostream& operator<<(std::ostream& out, const Tile& point) {
    out << "(" << point.x << ", " << point.y << ")";
    return out;
  }

  long p1(const std::vector<Rect>& rects) {
    std::vector rects_mutable { rects };
    std::ranges::sort(rects_mutable, [](const Rect& x, const Rect& y) { return x.area > y.area; });

    #if VERBOSE
    for (const auto& rect : rects_mutable) {
      std::cout << rect.idx_a << ", " << rect.idx_b << ": " << rect.area << "\n";
    }
    #endif

    return rects_mutable[0].area;
  }

  long left(const Tile& a, const Tile& b) { return std::min(a.x, b.x); }
  long right(const Tile& a, const Tile& b) { return std::max(a.x, b.x); }
  long top(const Tile& a, const Tile& b) { return std::max(a.y, b.y); }
  long bottom(const Tile& a, const Tile& b) { return std::min(a.y, b.y); }

  // Uses AABB collision detection, meaning it won't work on all inputs. Official day 9 inputs will probably be fine.
  bool collides(const std::vector<Tile>& reds, const Rect& rect) {
    const Tile& a0 { reds[rect.idx_a] };
    const Tile& b0 { reds[rect.idx_b] };

    // Looking through all the edges (the border of green tiles).
    for (int index_0 {}; index_0 < reds.size(); index_0++) {
      int index_1 { index_0 + 1 };
      if (index_0 == reds.size() - 1)
        index_1 = 0; // It loops back to the start.

      // No need to check these.
      if (index_0 == rect.idx_a || index_1 == rect.idx_a || index_0 == rect.idx_b || index_1 == rect.idx_b)
        continue;

      const Tile& a1 { reds[index_0] };
      const Tile& b1 { reds[index_1] };

      if (
        left(a0, b0) < right(a1, b1) &&
        right(a0, b0) > left(a1, b1) &&
        top(a0, b0) > bottom(a1, b1) &&
        bottom(a0, b0) < top(a1, b1)
      ) { return true; }
    }

    return false;
  }

  long p2(const std::vector<Tile>& reds, const std::vector<Rect>& rects) {
    long sol {};

    for (const Rect& r : rects) {
      if (collides(reds, r)) continue;
      if (r.area > sol) sol = r.area;
    }

    return sol;
  }
}

int day9(File& in) {
  using namespace Day9;

  std::vector<Tile> reds;
  std::vector<Rect> possible_rects {};

  std::string line {};
  while (in.next_line(line)) {
    Tile p {};
    p.x = std::stoi(line.substr(0, line.find(',')));
    p.y = std::stoi(line.substr(line.find(',') + 1));

    reds.emplace_back(p);
  }

  for (int i {}; i < reds.size(); i++)
    for (int j { i + 1 }; j < reds.size(); j++)
      possible_rects.emplace_back(i, j, area(reds[i], reds[j]));

  const long sol1 { p1(possible_rects) };
  std::cout << "PART 1: Largest rectangle has area " << sol1 << "\n";
  const long sol2 { p2(reds, possible_rects) };
  std::cout << "PART 2: Largest rectangle has area " << sol2 << "\n";

  return 0;
}
