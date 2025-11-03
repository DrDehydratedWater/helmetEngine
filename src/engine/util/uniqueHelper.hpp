#include <vector>
#include <memory>

template<typename T, typename... Args>
std::vector<std::unique_ptr<T>> make_unique_vector(Args&&... args) {
  std::vector<std::unique_ptr<T>> vec;
  vec.reserve(sizeof...(args));
  (vec.emplace_back(std::forward<Args>(args)), ...);
  return vec;
}