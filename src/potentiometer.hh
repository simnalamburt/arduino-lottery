#pragma once
#include "Arduino.h"

//
// Wrapper for a potentiometer. It normalize its input from integral number
// between [0, 1024) to floating point number between [0.0, 1.0)
//
template<uint8_t pin>
struct Potentiometer {
  double get() const {
    const auto raw = analogRead(pin);
    return raw / 1024.0;
  }
};
