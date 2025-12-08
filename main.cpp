#include <iostream>
#include <sstream>

#include "file.h"
#include "h/day1.h"
#include "h/day2.h"
#include "h/day3.h"
#include "h/day4.h"
#include "h/day5.h"
#include "h/day6.h"
#include "h/day7.h"
#include "h/day8.h"

int run_day(int i, File& in) {
  switch (i) {
    case 1:
      return day1(in);
    case 2:
      return day2(in);
    case 3:
      return day3(in);
    case 4:
      return day4(in);
    case 5:
      return day5(in);
    case 6:
      return day6(in);
    case 7:
      return day7(in);
    case 8:
      return day8(in);
    default: return 1;
  }
};

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: (program name) (day) (sample|input)\n";
    return 1;
  }

  // Convert day (arg 1) to int
  int day { -1 };
  std::string str { argv[1] };
  auto day_number { str.find_first_of("1234567890") };
  if (day_number == std::string::npos) {
    std::cerr << "No integer in '" << argv[1] << "'\n";
    return 1;
  }
  day = std::stoi(str.substr(day_number));

  // Create input file
  File in;
  try {
    in = File { day, argv[2] };
  } catch (int ex) {
    return ex;
  }

  return run_day(day, in);
}
