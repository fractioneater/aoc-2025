#ifndef AOC_2025_FILE_H
#define AOC_2025_FILE_H
#include <fstream>

class File {
  std::ifstream in_ {};

public:
  File() = default;

  explicit File(int day, const char* type) {
    in_ = std::ifstream { std::format("../{}/day{}.txt", type, day) };
    if (!in_) {
      std::cerr << std::format("Can't open {} file.\n", type);
      throw 1;
    }
  }

  bool next_line(std::string& line) {
    // For some reason I can't just use "return std::getline..."
    if (std::getline(in_, line)) return true;
    return false;
  }

  bool until_char(std::string& line, char c) {
    // For some reason I can't just use "return std::getline..."
    if (std::getline(in_, line, c)) return true;
    return false;
  }
};

#endif // AOC_2025_FILE_H
