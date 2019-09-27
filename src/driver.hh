#pragma once

#include <vector>

#include <units.h>

using namespace units;
using namespace units::literals;
using namespace units::time;

struct Pause {
  Pause(time::minute_t interval, time::minute_t duration)
    : interval(interval)
    , duration(duration)
  {}
  time::minute_t interval;
  time::minute_t duration;
};

class Driver {
public:
  Driver();
  time::minute_t time_to_next_pause() const;
  void drive_for(const time::minute_t);
  time::minute_t take_a_pause();
  time::minute_t time_before_pause() const;

  std::vector<Pause> pauses;
  std::vector<Pause>::iterator it;
  // Driving time since last pause
  time::minute_t driving_time;
};
