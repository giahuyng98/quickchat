#pragma once

#include <utility>
#include <vector>

template <class Self, class KeyType, class ValueType> class SessionMgr {
public:
  bool store(const KeyType &key, const ValueType &value) {
    return static_cast<Self &>(*this).storeImpl(key, value);
  }

  bool remove(const KeyType &key, const ValueType &value) {
    return static_cast<Self &>(*this).removeImpl(key, value);
  }

  const std::vector<ValueType> &getList(const KeyType &key) {
    return static_cast<Self &>(*this).getListImpl(key);
  }

};
