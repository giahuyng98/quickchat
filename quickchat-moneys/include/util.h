#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <charconv>
#include <fstream>
#include <nlohmann/json.hpp>

namespace util {
  nlohmann::json getConfig(const std::string &fileName) {
  std::ifstream ifs(fileName);
  nlohmann::json config;
  ifs >> config;
  return config;
};

int64_t toInt(std::string_view str) {
  int64_t result;
  std::from_chars(str.begin(), str.end(), result);
  return result;
};
}  // namespace util
#endif
