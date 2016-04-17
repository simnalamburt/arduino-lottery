#include "Arduino.h"

namespace {
  // Digital pin numbers of connected LEDs
  constexpr uint8_t LEDs[] { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };

  constexpr auto meter = []{ return analogRead(A0); };
  auto x0 = meter();
  auto t0 = micros();
}

void setup() {
  for (const auto led: LEDs) { pinMode(led, OUTPUT); }

  Serial.begin(9600);
}

void loop() {
  const auto x = meter();
  const auto dx = x - x0; x0 = x;

  const auto t = micros();
  const auto dt = t - t0; t0 = t;

  const auto v = (double(dx) / 1024.0)/(double(dt) / 1000000.0);

  const auto moved = v <= -1.0 || 1.0 < v;

  if (moved) {
    Serial.println(v);
  }

  const auto lit = moved ? HIGH : LOW;
  for (const auto led: LEDs) { digitalWrite(led, lit); }

  delay(100);
}
