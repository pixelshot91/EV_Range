#pragma once

#include <charging_curve.hh>

#include <map>
#include <units.h>
using namespace units;
using namespace units::energy;

class Battery {
public:
  Battery(energy::kilowatt_hour_t capacity, ChargingCurve max_power_cc);

	void set_to_full();
	void discharge(energy::kilowatt_hour_t e);
	void charge_for(time::minute_t duration, power::kilowatt_t max_charger_power);
	time::minute_t charge_to(Soc soc_goal, power::kilowatt_t max_charger_power);
	Soc soc() const;

  soc_interval get_soc_interval_for(const energy::watt_hour_t energy_to_gain, const power::kilowatt_t max_charger_power) const;
  time::minute_t get_time_to_recharge(const soc_interval, const power::kilowatt_t max_charger_power) const;
  time::minute_t get_time_to_recharge(const energy::watt_hour_t energy_to_gain, const Soc minimum_soc, const power::kilowatt_t max_charger_power) const;

  const ChargingCurve& get_cc(power::kilowatt_t max_power_power) const;
  const energy::kilowatt_hour_t capacity;
	energy::kilowatt_hour_t energy;

  const ChargingCurve maximum_cc;
  // Charging curve in function of max power output for cache
  mutable std::map<power::kilowatt_t, ChargingCurve> cc_map;
};
