#include <vehicle.hh>

using namespace units::literals;
using namespace units::energy_consumption;

Vehicle::Vehicle(std::string name, energy::kilowatt_hour_t battery_capacity, charging_curve_key_points_t charging_curve_key_points, EnConCurve en_con_curve)
  : name(name)
  , battery_capacity(battery_capacity)
  , max_power_cc(charging_curve_key_points)
  , en_con_curve(en_con_curve)
{}

time::minute_t Vehicle::get_time_to_recharge(const length::kilometer_t range_to_gain, const power::kilowatt_t max_charger_output) const
{
  ChargingCurve actual_cc = max_power_cc.min(max_charger_output);
  auto energy_to_gain = en_con_curve.get_consumption_at_speed(100._kph) * range_to_gain;
  if (energy_to_gain > battery_capacity)
    throw std::invalid_argument("Battery not big enough to hold " + length::to_string(range_to_gain) + " worth of energy");
  auto t = actual_cc.get_time_to_recharge(energy_to_gain, battery_capacity);
  return t;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& v)
{
  os << v.name;
  return os;
}
