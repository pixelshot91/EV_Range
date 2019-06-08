#pragma once
#include <charging_curve.hh>
#include <en_con_curve.hh>

#include "units.h"

using namespace units;

class Vehicle {
public:
  Vehicle(std::string name, energy::kilowatt_hour_t battery_capacity, charging_curve_key_points_t key_points);

  time::minute_t get_time_to_recharge(const length::kilometer_t range_to_gain, const power::kilowatt_t max_charger_output);

  friend std::ostream& operator<<(std::ostream& os, const Vehicle& v);

  const std::string name;
  const energy::kilowatt_hour_t battery_capacity;
  ChargingCurve max_power_cc;
  EnConCurve en_con_curve;
};
