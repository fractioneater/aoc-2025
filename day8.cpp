#include "h/day8.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "file.h"

#define VERBOSE 0

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

  int union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
      if (size[a] < size[b]) std::swap(a, b);
      parent[b] = a;
      size[a]   += size[b];

      // THIS was the line that caused the problem! Well, the problem was present because this line didn't exist.
      // One circuit was unioned with another but its old size was high enough to make top three
      size[b] = 0; // This circuit is gone.
      #if VERBOSE
      std::cout << "  " << b << " circuit has joined the " << a << " circuit (new size " << size[a] << ")\n";
      #endif
      return size[a];
    }
    return 0;
  }
};

struct Box {
  int x {}, y {}, z {};
};

long sq_distance(const Box& a, const Box& b) {
  const long dx { a.x - b.x };
  const long dy { a.y - b.y };
  const long dz { a.z - b.z };
  return dx * dx + dy * dy + dz * dz;
}

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

long p1(const int sort_count, const std::vector<Box>& boxes, std::vector<Edge>& edges) {
  std::ranges::partial_sort(edges, edges.begin() + sort_count, [](const Edge& x, const Edge& y) { return x.distance < y.distance; });

  Dsu dsu { static_cast<int>(boxes.size()) };
  for (int i {}; i < sort_count; ++i) {
    #if VERBOSE
    std::cout << i + 1 << "th shortest edge from " << edges[i].a << " to " << edges[i].b << ", distance: " << edges[i].distance << "\n";
    #endif
    dsu.union_sets(edges[i].a, edges[i].b);
  }

  std::ranges::partial_sort(dsu.size, dsu.size.begin() + 3, std::greater());

  #if VERBOSE
  std::cout << "Top three unions are " << dsu.size[0] << ", " << dsu.size[1] << ", and " << dsu.size[2] << "\n";
  #endif
  // I should be getting 26, 52, 73.
  return dsu.size[0] * dsu.size[1] * dsu.size[2];
}

long p2(const std::vector<Box>& boxes, std::vector<Edge>& edges) {
  std::ranges::sort(edges, [](const Edge& x, const Edge& y) { return x.distance < y.distance; });

  long long sol {};
  Dsu dsu { static_cast<int>(boxes.size()) };
  for (const auto& edge : edges) {
    if (dsu.union_sets(edge.a, edge.b) == boxes.size()) {
      #if VERBOSE
      std::cout << boxes[edge.a].x << " * " << boxes[edge.b].x << "\n";
      #endif
      sol = static_cast<long>(boxes[edge.a].x) * static_cast<long>(boxes[edge.b].x);
      break;
    }
  }

  return sol;
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
    for (int j { i + 1 }; j < boxes.size(); j++)
      edges.emplace_back(i, j, sq_distance(boxes[i], boxes[j]));

  const long sol1 = p1(sort_count, boxes, edges);
  std::cout << "PART 1: Multiplied set sizes is " << sol1 << "\n";
  const long sol2 = p2(boxes, edges);
  std::cout << "PART 2: X coordinates multiplied is " << sol2 << "\n";

  return 0;
}
