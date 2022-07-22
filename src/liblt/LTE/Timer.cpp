#include "Timer.h"

#include "SFML/System.hpp"

struct TimerData {
  sf::Clock clock;
};

Timer::Timer() {
  d = new TimerData;
}

Timer::~Timer() {
  delete d;
}

float Timer::GetElapsed() const {
  return d->clock.getElapsedTime().asSeconds();
}

void Timer::Reset() {
  d->clock.restart();
}
