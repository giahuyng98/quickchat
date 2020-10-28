#include "util.h"

namespace util {
int64_t toInt(std::string_view str) {
  int64_t result{};
  std::from_chars(str.begin(), str.end(), result);
  return result;
};

std::vector<int64_t> strVecToInt(const std::vector<std::string> &vec) {
  std::vector<int64_t> result;
  result.reserve(vec.size());
  std::transform(vec.begin(), vec.end(), result.begin(), util::toInt);
  return result;
}
bool is_uuid(const std::string_view uuid) {
  constexpr int UUID_LENGTH = 36;
  constexpr std::array<int, 4> hypens = {8, 13, 18, 23};

  if (uuid.length() != UUID_LENGTH) {
    return false;
  }

  for (size_t i = 0; i < UUID_LENGTH; ++i) {
    if (std::find(std::begin(hypens), std::end(hypens), i) !=
        std::end(hypens)) {
      if (uuid[i] != '-') {
        return false;
      }
    } else {
      if (!std::isxdigit(uuid[i])) {
        return false;
      }
    }
  }
  return true;
}
} // namespace util
