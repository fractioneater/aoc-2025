#include "h/day8.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "file.h"

#define VERBOSE 1
#define PART 1

struct Dsu {
  std::vector<int> parent {}, size {};

  explicit Dsu(int n) : parent(n), size(n, 1) {
    for (int i {}; i < parent.size(); ++i) parent[i] = i;
  }

  void make_set(int v) {
    parent[v] = v;
  }

  int find_set(int v) {
    if (v == parent[v])
      return v;
    return parent[v] = find_set(parent[v]);
  }

  void union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
      if (size[a] < size[b]) std::swap(a, b);
      parent[b] = a;
      size[a] += size[b];
      #if VERBOSE
      std::cout << "  Size of the " << a << " union is now " << size[a] << "\n";
      #endif
    }
  }
};

struct Box {
  int x {}, y {}, z {};

  [[nodiscard]] double distance(const Box& other) const {
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2));
  }
};

struct Edge {
  int a, b;
  double distance {};

  Edge(int a, int b, double distance) : a { a }, b { b }, distance { distance } {}

  Edge& operator=(const Edge& copy) {
    if (&copy == this) return *this;

    a        = copy.a;
    b        = copy.b;
    distance = copy.distance;

    return *this;
  }
};

std::ostream& operator<<(std::ostream& out, const Box& point) {
  out << "(" << point.x << ", " << point.y << ", " << point.z << ")";
  return out;
}

int day8(File& in) {
  // IMPORTANT: Set this to 10 for sample, 1000 for input.
  constexpr int sort_count { 1000 };

  // PLAN: Kruskal's with some union finding (which I just learned about).
  // https://www.tutorialspoint.com/data_structures_algorithms/kruskals_spanning_tree_algorithm.htm

  std::vector<Box> boxes;
  std::vector<Edge> edges {};

  std::string line {};
  while (in.next_line(line)) {
    Box p {};
    p.x = std::stoi(line.substr(0, line.find(',')));
    p.y = std::stoi(line.substr(line.find(',') + 1, line.find_last_of(',')));
    p.z = std::stoi(line.substr(line.find_last_of(',') + 1));

    boxes.emplace_back(p);
  }

  for (int i {}; i < boxes.size(); i++)
    for (int j { i + 1 }; j < boxes.size(); j++) {
      edges.emplace_back(i, j, boxes[i].distance(boxes[j]));
    }

  std::ranges::partial_sort(edges, edges.begin() + sort_count, [](const Edge& x, const Edge& y) { return x.distance < y.distance; });

  Dsu dsu { static_cast<int>(boxes.size()) };
  for (int i {}; i < sort_count; ++i) {
    #if VERBOSE
    std::cout << i + 1 << "th short edge from " << edges[i].a << " to " << edges[i].b << ", distance: " << edges[i].distance << "\n";
    #endif
    dsu.union_sets(edges[i].a, edges[i].b);
  }

  std::ranges::partial_sort(dsu.size, dsu.size.begin() + 3, std::greater());

  #if VERBOSE
  std::cout << "Top three unions are " << dsu.size[0] << ", " << dsu.size[1] << ", and " << dsu.size[2] << "\n";
  #endif
  const long top_3 { dsu.size[0] * dsu.size[1] * dsu.size[2] };

  std::cout << "PART: " << PART << "\nMultiplied set sizes is " << top_3 << "\n";

  return 0;
}
