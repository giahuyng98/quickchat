#ifndef BODY_PARSER_H
#define BODY_PARSER_H

#include <algorithm>
#include <optional>
#include <string_view>

class BodyParser {
public:
  using KeyType = std::string_view;
  using ValueType = std::string_view;
  using ElemType = std::pair<KeyType, ValueType>;

  explicit BodyParser(std::string_view str);
  std::optional<ValueType> operator[](KeyType key) const;

private:
  std::vector<ElemType> body;
};

#endif
