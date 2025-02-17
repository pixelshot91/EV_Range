#pragma once
#include <soc.hh>

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

struct soc_interval {
	soc_interval(Soc low = Soc(0), Soc high = Soc(0))
		: low(low), high(high) {}
  Soc low;
  Soc high;
};

using charging_curve_key_points_t = std::vector<charging_key_point_t>;
using charging_curve_points_t = std::array<power::kilowatt_t, 101>;
using optimal_soc_intervals_t = std::array<soc_interval, 101>;

class ChargingCurve {
private:
  void init_soc_intervals();
public:
  ChargingCurve(charging_curve_key_points_t key_points);
  ChargingCurve(std::array<power::kilowatt_t, 101> points);

  ChargingCurve min(const power::kilowatt_t max_charger_output) const;
  time::minute_t get_time_to_recharge_soc(soc_interval soc_interval, energy::watt_hour_t battery_capacity) const;
  soc_interval get_soc_interval_for(const energy::watt_hour_t energy_to_gain, energy::watt_hour_t battery_capacity) const;
	energy::kilowatt_hour_t energy_gained_from_charging(Soc soc, time::minute_t duration, energy::watt_hour_t battery_capacity) const;

  charging_curve_points_t points;
	power::kilowatt_t peak_power;
  optimal_soc_intervals_t soc_intervals;
};
