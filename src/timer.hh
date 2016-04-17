#pragma once
#include "Arduino.h"

//
// 타이머 클래스입니다.
//
//     Timer timer;
//     timer.stopped(); // true
//
//     timer.millisecond(1000);
//     timer.stopped(); // false
//
//     void loop() {
//       timer.onLoop();
//     }
//
//     /* 10초 후 */
//     timer.stopped(); // true
//
struct Timer {
  using type = double;

private:
  bool _stopped = true;
  decltype(millis()) target;

public:
  void second(int second) { millisecond(second*1000); }
  void millisecond(int millisecond) {
    _stopped = false;
    target = millis() + millisecond;
  }

  void onLoop() {
    if (_stopped) { return; }
    if (target <= millis()) { _stopped = true; }
  }

  bool stopped() const { return _stopped; }
};
