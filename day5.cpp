#include "h/day5.h"

#include <iostream>

#include "file.h"

#define VERBOSE_RANGES 0
#define VERBOSE_INGREDIENTS 1
#define PART 2

struct RangeElement {
  enum Type { BEGIN, END, NONE };

  Type type { NONE };
  long id { -1 };
  RangeElement* next { nullptr }; // Linked list!

  ~RangeElement() {
    #if VERBOSE_RANGES
    std::cout << (type == BEGIN ? "BEGIN." : (type == END ? "END." : "\n")) << id << " destructed\n";
    #endif
    delete next;
  }
};

/* Alright, part 1 planning time. I'm going to create a linked list (SORTED) of RangeElements that can either be a BEGIN
 * or an END. To check each ingredient, it's as simple as going through the sorted list and counting nesting.
 */

void insert(RangeElement* after, RangeElement* const element) {
  const long id = element->id;
  RangeElement* before { after };
  while (after != nullptr && id > after->id) { // While the new element's ID is greater than the ID of the element that's supposed to be after it,
    before = after;                            // in case it's correct, store the element before it, and
    after  = after->next;                      // increment after.
  }
  if (before->next != nullptr) element->next = after;
  before->next = element;
}

int day5(File& in) {
  RangeElement fake_range { RangeElement::NONE, -1, nullptr };
  RangeElement* list { &fake_range };

  std::string line {};

  // Range parsing:
  while (in.next_line(line)) {
    if (line.empty()) {
      #if VERBOSE_RANGES
      std::cout << "End of ranges\n\n";
      #endif
      break;
    }

    auto pos = line.find('-');
    if (pos == std::string::npos) return 1;

    long begin_id { std::stol(line.substr(0, pos)) };
    long end_id { std::stol(line.substr(pos + 1)) };

    #if VERBOSE_RANGES
    std::cout << begin_id << " to " << end_id << '\n';
    #endif

    auto new_begin { new RangeElement { RangeElement::BEGIN, begin_id } };
    auto new_end { new RangeElement { RangeElement::END, end_id } };
    insert(list, new_begin);
    insert(list, new_end);
  }

  #if PART == 1
  // AVAILABLE ingredient checking:
  int fresh { 0 };

  while (in.next_line(line)) {
    long id { std::stol(line) };
    #if VERBOSE_INGREDIENTS
    std::cout << "Ingredient " << id << '\n';
    #endif

    int nesting { 0 };
    auto r_e { list };
    while (r_e != nullptr && id > r_e->id) {
      switch (r_e->type) {
        case RangeElement::BEGIN:
          nesting++;
          break;
        case RangeElement::END:
          nesting--;
          break;
        default: ; // Nothing.
      }

      #if VERBOSE_INGREDIENTS
      std::cout << "  Comparing with " << r_e->id << ": nesting " << nesting << '\n';
      #endif
      r_e = r_e->next;
    }
    if (r_e && (r_e->id == id || nesting > 0)) {
      #if VERBOSE_INGREDIENTS
      std::cout << "  FRESH\n";
      #endif
      fresh++;
    }
  }

  #elif PART == 2
  // TOTAL ingredient checking:
  long fresh { 0 };

  int nesting { 0 };
  long opening_id {};
  auto r_e { list };
  while (r_e != nullptr) {
    switch (r_e->type) {
      case RangeElement::BEGIN:
        if (nesting == 0) {
          #if VERBOSE_INGREDIENTS
          std::cout << "Start of fresh range group: " << r_e->id << '\n';
          #endif
          opening_id = r_e->id;
        }
        nesting++;
        break;
      case RangeElement::END:
        nesting--;
        if (nesting == 0) {
          #if VERBOSE_INGREDIENTS
          std::cout << "End of fresh range group: " << r_e->id << '\n';
          #endif
          fresh += r_e->id - opening_id + 1; // +1 for closed range
        }
        break;
      default: ; // Nothing.
    }
    r_e = r_e->next;
  }
  #endif

  std::cout << "PART: " << PART << "\n" << fresh << " fresh ingredients\n";

  // The destructor should get clear out the whole linked list.
  return 0;
}
