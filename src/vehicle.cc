#include <vehicle.hh>

#include <tools.hh>

using namespace units::literals;
using namespace units::energy_consumption;
using namespace units::length;

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

std::vector<time::second_t> Vehicle::get_time_for_distance(const length::kilometer_t max_distance, const length::kilometer_t distance_increment, const length::kilometer_t distance_between_charger, const velocity::kilometers_per_hour_t cruising_speed) const
{
  std::vector<time::second_t> times;
  for (length::kilometer_t distance(0); distance <= max_distance; distance += distance_increment) {
    auto time = time_to_do_trip(distance, distance_between_charger, cruising_speed);
    std::cout << "distance = " << distance << " -> " << tools::pretty_print(time) << std::endl;
    times.push_back(time);
  }
  return times;
}

time::second_t Vehicle::time_to_do_trip(const length::kilometer_t distance_goal, const length::kilometer_t distance_between_charger, const velocity::kilometers_per_hour_t cruising_speed) const
{
  bool debug = true;
  auto charger_detour = 5._min;
  auto max_charger_power = 350._kW;
  auto en_con = en_con_curve.get_consumption_at_speed(cruising_speed);
  auto range = battery_capacity / en_con;
  std::cout << "range = " << range << std::endl;
  if (range > distance_goal)
    return distance_goal / cruising_speed;
  energy::kilowatt_hour_t battery_energy = battery_capacity;

  //auto distance_driven = tools::round_down(length::kilometer_t(battery_energy / en_con), distance_between_charger);
  if (distance_goal < 2 * range) {
    std::cout << "distance_goal < 2 * range" << std::endl;
    auto charging_time = get_time_to_recharge(distance_goal - range, max_charger_power);
    return distance_goal / cruising_speed + charging_time + charger_detour;
  }
  double low_soc = max_power_cc.get_soc_interval_for(distance_between_charger * en_con, battery_capacity).low;
  std::cout << "low_soc = " << low_soc << std::endl;
  length::kilometer_t distance_driven = battery_capacity * (1.0 - (low_soc / 100.0)) / en_con;
  //const int soc_stop_charging = 80;
  auto time = distance_driven / cruising_speed;
  std::cout << "First leg: " << distance_driven << " in " << tools::pretty_print(time) << std::endl;
  while (distance_driven < distance_goal - distance_between_charger) {
    auto charging_time = get_time_to_recharge(distance_between_charger, max_charger_power);
    if (debug) std::cout << "Charging stop for " << tools::pretty_print(charging_time) << std::endl;
    time += charging_time + charger_detour;

    distance_driven += distance_between_charger;
    time += distance_between_charger / cruising_speed;
    if (debug) std::cout << "Driving " << distance_between_charger << " in " << tools::pretty_print(distance_between_charger / cruising_speed) << std::endl;
  }
  auto distance_last_leg = distance_goal - distance_driven;
  auto charging_time = get_time_to_recharge(distance_last_leg, max_charger_power);
  if (debug) std::cout << "Last Charging stop for " << tools::pretty_print(charging_time) << std::endl;
  time += charging_time + charger_detour;

  time += distance_last_leg / cruising_speed;
  distance_driven += distance_last_leg;
  if (debug) std::cout << "Total distance driven = " << distance_driven << " in " << tools::pretty_print(time) << std::endl;
  return time;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& v)
{
  os << v.name;
  return os;
}
