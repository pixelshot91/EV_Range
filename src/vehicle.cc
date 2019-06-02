#include <vehicle.hh>

Vehicle::Vehicle(std::string name, energy::kilowatt_hour_t battery_capacity, charging_curve_key_points_t charging_curve_key_points)
  : name(name)
  , battery_capacity(battery_capacity)
  , max_power_charging_curve(charging_curve_key_points)
{}
