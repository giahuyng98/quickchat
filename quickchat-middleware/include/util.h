#ifndef UTIL_H
#define UTIL_H

#include <charconv>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>

namespace util {
nlohmann::json getConfig(const std::string &fileName);

int64_t toInt(std::string_view str);

std::vector<int64_t> strVecToInt(const std::vector<std::string> &vec);

bool is_uuid(const std::string_view uuid);
//std::vector<std::string_view, std::string_view> parseBody(std::string_view body);

} // namespace util
#endif
