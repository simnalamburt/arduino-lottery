#pragma once
#include <stddef.h>

namespace util {
  template<typename T, size_t N>
  constexpr size_t length(T (&array)[N]) { return N; }

  template<typename T>
  class Iterator {
    T _it;
  public:
    const T step;

    Iterator(T it, T step) : _it(it), step(step) { }
    bool operator!=(const Iterator<T> &right) const { return **this < *right; }
    T operator*() const { return _it; }
    Iterator& operator++() { _it += step; return *this; }
  };

  template<typename T>
  struct Range {
    const T _begin, _end, _step;
    Iterator<T> begin() const { return { _begin, _step }; }
    Iterator<T> end() const { return { _end, _step }; }
  };

  // [begin, end) 범위를 갖는 range를 만듬
  template<typename T>
  Range<T> range(T begin, T end, T step = 1) {
    return { begin, end, step };
  }
}
