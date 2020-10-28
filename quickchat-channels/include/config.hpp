#pragma once

#include <string_view>

template <class T> class Config {
public:
  void loadFromFile(const std::string_view fileName) {
    static_cast<T &>(*this).loadFromFileImpl(fileName);
  }

  template <class KeyType> auto &operator[](const KeyType &key) const {
    return static_cast<T &>(*this)[key];
  }

};

