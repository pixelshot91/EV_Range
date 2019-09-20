#pragma once

#include <charging_curve.hh>

#include <map>
#include "units.h"
using namespace units;

class Battery {
public:
  Battery(energy::kilowatt_hour_t capacity, ChargingCurve max_power_cc);

  soc_interval get_soc_interval_for(const energy::watt_hour_t energy_to_gain, const power::kilowatt_t max_charger_power) const;
  time::minute_t get_time_to_recharge(const soc_interval, const power::kilowatt_t max_charger_power) const;
  time::minute_t get_time_to_recharge(const energy::watt_hour_t energy_to_gain, const double minimum_soc, const power::kilowatt_t max_charger_power) const;

  const ChargingCurve& get_cc(power::kilowatt_t max_power_power) const;
  const energy::kilowatt_hour_t capacity;

  const ChargingCurve maximum_cc;
  // Charging curve in function of max power output for cache
  mutable std::map<power::kilowatt_t, ChargingCurve> cc_map;
  //mutable std::map<double, ChargingCurve> cc_map;
};
