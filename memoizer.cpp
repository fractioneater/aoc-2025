#include <functional>
#include <map>
#include <utility>

// Generic memoization wrapper using C++23 deduced this
template <typename Return_T, typename... Args>
class Memoizer {
  std::function<Return_T(Args...)> fn_;
  mutable std::map<std::tuple<Args...>, Return_T> cache_;

public:
  explicit Memoizer(std::function<Return_T(Args...)> f) : fn_(std::move(f)) {}

  Return_T operator()(this auto&& self, Args... args) {
    auto key = std::make_tuple(args...);
    auto it  = self.cache_.find(key);

    if (it != self.cache_.end()) {
      return it->second;
    }

    Return_T result  = self.fn_(args...);
    self.cache_[key] = result;
    return result;
  }

  void clear_cache(this auto&& self) {
    self.cache_.clear();
  }
};
