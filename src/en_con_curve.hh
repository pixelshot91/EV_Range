#pragma once

#include "units.h"
using namespace units;
using namespace units::energy_consumption;

using whkm = energy_consumption::Wh_per_km_t;

class EnConCurve {
public:
  EnConCurve(whkm v)
    : v(v)
  {}
  whkm get_consumption_at_speed(velocity::kilometers_per_hour_t /*s*/) const { return v; }
  whkm v;
};
