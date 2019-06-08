#include <vehicle.hh>

using namespace units::literals;
using namespace units::energy_consumption;

Vehicle::Vehicle(std::string name, energy::kilowatt_hour_t battery_capacity, charging_curve_key_points_t charging_curve_key_points)
  : name(name)
  , battery_capacity(battery_capacity)
  , max_power_cc(charging_curve_key_points)
  , en_con_curve(180_Whpkm)
{}

time::minute_t Vehicle::get_time_to_recharge(const length::kilometer_t range_to_gain, const power::kilowatt_t max_charger_output)
{
  ChargingCurve actual_cc = max_power_cc.min(max_charger_output);
  auto energy_to_gain = en_con_curve.get_consumption_at_speed(100._kph) * range_to_gain;
  auto t = actual_cc.get_time_to_recharge(energy_to_gain);
  return t;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& v)
{
  os << v.name;
  return os;
}
