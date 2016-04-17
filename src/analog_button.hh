#pragma once
#include "Arduino.h"

//
// Wrapper for an analog button. Performs rising edge detection.
//
template<uint8_t pin>
class AnalogButton {
  bool last_state = false;
  bool _clicked = false;

public:
  // `onLoop` should be called in `loop()` function. It updates given analog
  // button's input state.
  decltype(analogRead(pin)) onLoop() {
    const auto voltage = analogRead(pin);

    bool current_state;
    if (voltage >= 1024 / 8 * 7) {
      current_state = true;
    } else if (voltage < 1024 / 8) {
      current_state = false;
    } else {
      // Input voltage is currently unstable.
      // Skip this frame's input and return `voltage` right away.
      return voltage;
    }

    _clicked = !last_state && current_state;

    // Update state
    last_state = current_state;
    return voltage;
  }

  // Returns `true` if given analog button was clicked at the last `onLoop()`
  // call. On otherwhise, returns `false`.
  bool clicked() const { return _clicked; }
};
