#pragma once

#include <array>
#include <vector>

#include "units.h"
using namespace units;

struct charging_key_point_t {
  charging_key_point_t(int soc, power::kilowatt_t power)
    : soc(soc)
    , power(power)
  {}

  int soc;
  power::kilowatt_t power;
};

using charging_curve_key_points_t = std::vector<charging_key_point_t>;

class ChargingCurve {
public:
  ChargingCurve(charging_curve_key_points_t key_points);
  ChargingCurve(std::array<power::kilowatt_t, 101> points);
  ChargingCurve min(const power::kilowatt_t max_charger_output) const;
  time::minute_t get_time_to_recharge(energy::watt_hour_t energy_to_gain);

  std::array<power::kilowatt_t, 101> points;
};
