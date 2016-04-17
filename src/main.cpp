#include "util.hh"
#include "timer.hh"
#include "analog_button.hh"
#include "potentiometer.hh"
#include "Arduino.h"

using util::length;
using util::range;

namespace {
  // Input hardwares
  Potentiometer<A0> meter;
  AnalogButton<A1> left;
  AnalogButton<A2> right;

  // Digital pin numbers of connected LEDs
  constexpr uint8_t LEDs[] { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

  // Program state
  enum class Mode { Blinking, Number, Sleeping };
  Mode mode = Mode::Sleeping;
  Timer timer;

  decltype(HIGH) blinkstates[length(LEDs)] = {};
  Timer blinktimer;

  constexpr decltype(random(0)) dice_upperbound = 6;
  decltype(random(0)) dice_result;

  // Roll dice
  void roll() {
    timer.millisecond(1500);
    mode = Mode::Blinking;
  }

  // Stop dice
  void stop() {
    dice_result = random(dice_upperbound);
    timer.second(5);
    mode = Mode::Number;
  }

  // 문제 1번 답안
  void logic() {
    const auto l = left.last_state(),
               r = right.last_state();

    for (const auto i: range(0u, length(LEDs))) {
      digitalWrite(LEDs[i], (i%2 ? l||r : l&&r) ? HIGH : LOW);
    }
  }
}

void setup() {
  Serial.begin(9600);
  for (const auto led: LEDs) { pinMode(led, OUTPUT); }

  roll();
}

void loop() {
  // Need to be called once a every loop
  timer.onLoop();
  blinktimer.onLoop();
  left.onLoop();
  right.onLoop();


  // 포텐시오미터가 한쪽 끝으로 꺾여있을경우, 다른 과제 프로그램으로 변신함
  const auto val = meter.get();
  if (0.9 < val) { return logic(); }


  // Left button rolls the dice
  if (left.clicked()) { roll(); }
  // Right button forces dice to stop
  if (mode == Mode::Blinking && right.clicked()) { stop(); }

  switch (mode) {
  case Mode::Blinking:
    for (const auto i: range(0u, length(LEDs))) {
      digitalWrite(LEDs[i], blinkstates[i]);
    }

    if (blinktimer.stopped()) {
      for (auto& state: blinkstates) { state = random(3) == 0 ? HIGH : LOW; }
      blinktimer.millisecond(60);
    }

    // Blinking mode is over
    if (timer.stopped()) { stop(); }
    break;
  case Mode::Number: {
    constexpr decltype(HIGH) matrix[dice_upperbound][length(LEDs)] = {
      { LOW,  HIGH, LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW },
      { HIGH, HIGH, HIGH, LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW },
      { HIGH, HIGH, HIGH, HIGH, LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  LOW,  HIGH },
      { HIGH, HIGH, HIGH, HIGH, HIGH, LOW,  LOW,  LOW,  LOW,  LOW,  HIGH, HIGH },
      { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW,  HIGH, LOW,  HIGH, HIGH, HIGH },
      { HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH },
    };

    const auto *dice = matrix[dice_result];
    for (const auto i: range(0u, length(LEDs))) { digitalWrite(LEDs[i], dice[i]); }

    // Number mode is over
    if (timer.stopped()) { mode = Mode::Sleeping; }
    break; }
  case Mode::Sleeping:
    for (const auto led: LEDs) { digitalWrite(led, LOW); }
    break;
  }
}
