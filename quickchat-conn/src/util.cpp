#include "util.h"

namespace util {
int64_t toInt(std::string_view str) {
  int64_t result;
  std::from_chars(str.begin(), str.end(), result);
  return result;
};

std::vector<int64_t> strVecToInt(const std::vector<std::string> &vec) {
  std::vector<int64_t> result;
  result.reserve(vec.size());
  std::transform(vec.begin(), vec.end(), result.begin(), util::toInt);
  return result;
}

} // namespace util
