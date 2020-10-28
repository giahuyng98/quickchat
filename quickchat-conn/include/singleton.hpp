#pragma once
#include <type_traits>
#include <utility>

// Do not use this class explicitly
// only use this class for inheritance
template <class T> class Singleton {

public:
  constexpr Singleton() = default;
  Singleton(const Singleton &) = delete;
  Singleton(Singleton &&) = delete;
  Singleton &operator=(const Singleton &) = delete;
  Singleton &operator=(Singleton &&) = delete;

  template <class... Args> static T &instance(Args &&... args) {
    static T instance(std::forward<Args>(args)...);
    return instance;
  }
  
};
