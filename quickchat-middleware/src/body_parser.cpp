#include "body_parser.hpp"
#include <string_view>

// TODO: test it, or use regex
BodyParser::BodyParser(std::string_view str) {

  size_t lastIdx = 0;
  bool isKey = 1;
  std::string_view key, value;
  for (size_t idx = 0; idx < str.size(); ++idx) {
    if (str[idx] == '=' || str[idx] == '&') {
      if (isKey) {
        key = str.substr(lastIdx, idx - lastIdx);
      } else {
        value = str.substr(lastIdx, idx - lastIdx);
        body.emplace_back(key, value);
      }
      isKey = !isKey;
      lastIdx = idx + 1;
    }
    if (idx == str.size() - 1) {
      value = str.substr(lastIdx, idx - lastIdx + 1);
      body.emplace_back(key, value);
    }
  }
}

std::optional<BodyParser::ValueType> BodyParser::operator[](KeyType key) const {
  const auto it =
      std::find_if(body.begin(), body.end(),
                   [&](const ElemType &e) { return e.first == key; });
  return it == body.end()
             ? std::nullopt
             : std::make_optional<BodyParser::ValueType>(it->second);
}
