#include <vehicle.hh>

#include <tools.hh>

using namespace units::literals;
using namespace units::energy_consumption;
using namespace units::length;
using namespace units::energy;
using namespace units::power;

Vehicle::Vehicle(std::string name, Battery battery, EnConCurve en_con_curve)
  : name(name)
  , battery(battery)
  , en_con_curve(en_con_curve)
{
}

/*time::minute_t Vehicle::get_time_to_recharge(const length::kilometer_t range_to_gain, const power::kilowatt_t max_charger_output) const
{
  ChargingCurve actual_cc = max_power_cc.min(max_charger_output);
  auto energy_to_gain = en_con_curve.get_consumption_at_speed(100._kph) * range_to_gain;
  if (energy_to_gain > battery_capacity)
    throw std::invalid_argument("Battery not big enough to hold " + length::to_string(range_to_gain) + " worth of energy");
  auto t = actual_cc.get_time_to_recharge(energy_to_gain, battery_capacity);
  return t;
}*/

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
  bool debug = false;
  auto charger_detour = 5._min;
  auto en_con = en_con_curve.get_consumption_at_speed(cruising_speed);
  auto range = battery.capacity / en_con;
  const Soc minimum_soc(0.10);
  auto usable_range = range * (1.0 - minimum_soc.as_ratio());
  if (debug) std::cout << "distance goal = " << distance_goal << ", range = " << range << std::endl;
  if (range > distance_goal)
    return distance_goal / cruising_speed;
  energy::kilowatt_hour_t battery_energy = battery.capacity;

  auto max_charger_power = 35099._kW;
  //ChargingCurve actual_cc = max_power_cc.min(max_charger_power);
  /*for (auto p: actual_cc.points)
    std::cout << p << std::endl;*/
  //auto distance_driven = tools::round_down(length::kilometer_t(battery_energy / en_con), distance_between_charger);
  if (distance_goal < 1.8 * usable_range) {
    if (debug) std::cout << "distance_goal < 2 * range" << std::endl;
    auto energy_needed = distance_goal * en_con;
    energy::kilowatt_hour_t energy_to_gain = energy_needed - battery.capacity * (1.0 - minimum_soc.as_ratio());
    if (debug) std::cout << "energy_to_gain = " << energy_to_gain << std::endl;
    auto charging_time = battery.get_time_to_recharge(energy_to_gain, minimum_soc, max_charger_power);
    if (debug) std::cout << "Driving for " << tools::pretty_print(distance_goal / cruising_speed) << std::endl
      << "Charging for " << tools::pretty_print(charging_time) << " + "
      <<  tools::pretty_print(charger_detour) << " detour" << std::endl;
    return distance_goal / cruising_speed + charging_time + charger_detour;
  }
  auto soc_interval = battery.get_soc_interval_for(distance_between_charger * en_con, max_charger_power);
  if (debug) std::cout << "low_soc = " << soc_interval.low.str() << std::endl;

  length::kilometer_t distance_driven = battery.capacity * (1.0 - soc_interval.low.as_ratio()) / en_con;
  //const int soc_stop_charging = 80;
  auto time = distance_driven / cruising_speed;
  if (debug) std::cout << "First leg: " << distance_driven << " in " << tools::pretty_print(time) << std::endl;

  // We are at soc_interval.low
  while (distance_driven + soc_interval.high.as_ratio() * battery.capacity / en_con < distance_goal
      && distance_driven < distance_goal - distance_between_charger) {

    auto charging_time = battery.get_time_to_recharge(distance_between_charger * en_con, minimum_soc, max_charger_power);
    if (debug) std::cout << "Charging stop for " << tools::pretty_print(charging_time) << std::endl;
    time += charging_time + charger_detour;

    distance_driven += distance_between_charger;
    time += distance_between_charger / cruising_speed;
    if (debug) std::cout << "Driving " << distance_between_charger << " in " << tools::pretty_print(distance_between_charger / cruising_speed) << std::endl;
  }
  // We are at soc_interval.low
  auto distance_last_leg = distance_goal - distance_driven;
  auto needed_energy = distance_last_leg * en_con;
  battery_energy = soc_interval.low.as_ratio() * battery.capacity;
  auto missing_energy = needed_energy - battery_energy;
  Soc goal_soc(needed_energy / battery.capacity);
  if (missing_energy > 0._kWh) {
    if (debug) std::cout << "goal soc is " << goal_soc.str() << std::endl;
    struct soc_interval si = {soc_interval.low, goal_soc};

    auto charging_time = battery.get_time_to_recharge(si, max_charger_power);
    if (debug) std::cout << "Last Charging stop for " << tools::pretty_print(charging_time) << std::endl;
    time += charging_time + charger_detour;
  }

  auto last_leg_time = distance_last_leg / cruising_speed;
  if (debug) std::cout << "Last leg: " << distance_last_leg << " in " << tools::pretty_print(last_leg_time) << std::endl;
  time += last_leg_time;
  distance_driven += distance_last_leg;
  if (debug) std::cout << "Total distance driven = " << distance_driven << " in " << tools::pretty_print(time) << std::endl;
  return time;
}

std::ostream& operator<<(std::ostream& os, const Vehicle& v)
{
  os << v.name;
  return os;
}
