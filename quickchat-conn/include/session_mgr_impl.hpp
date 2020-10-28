#pragma once
#include "session_mgr.hpp"
#include <algorithm>
#include <unordered_map>
#include <utility>

template <class KeyType, class ValueType>
class SessionMgrImpl : public SessionMgr<SessionMgrImpl<KeyType, ValueType>,
                                         KeyType, ValueType> {
private:
  // TODO: possible duplicate ?
  // real implement
  friend class SessionMgr<SessionMgrImpl<KeyType, ValueType>, KeyType,
                          ValueType>;
  bool storeImpl(const KeyType &key, const ValueType &value) {
    map[key].emplace_back(value);
    return true;
  }

  bool removeImpl(const KeyType &key, const ValueType &value) {
    // TODO: key not exists ?
    auto &c = map[key];
    if (const auto it = std::find(begin(c), end(c), value); it != c.end()) {
      c.erase(it);
      return true;
    }
    return false;
  }

  const std::vector<ValueType> &getListImpl(const KeyType &key) const {
    if (const auto it = map.find(key); it != map.end()) {
      return it->second;
    }
    // TODO: Stupid, fix it
    static const std::vector<ValueType> EMPTY;
    return EMPTY;
  }

private:
  std::unordered_map<KeyType, std::vector<ValueType>> map;
};
