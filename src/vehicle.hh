#pragma once
#include <en_con_curve.hh>
#include <battery.hh>

#include "units.h"

using namespace units;

class Vehicle {
public:
  Vehicle(std::string name, Battery battery, EnConCurve en_con_curve);

  //time::minute_t get_time_to_recharge(const length::kilometer_t range_to_gain, const power::kilowatt_t max_charger_output) const;
  std::vector<time::second_t> get_time_for_distance(const length::kilometer_t max_distance, const length::kilometer_t distance_increment, const length::kilometer_t distance_between_charger, const velocity::kilometers_per_hour_t cruising_speed) const;
  time::second_t time_to_do_trip(const length::kilometer_t distance, const length::kilometer_t distance_between_charger, const velocity::kilometers_per_hour_t cruising_speed) const;

  friend std::ostream& operator<<(std::ostream& os, const Vehicle& v);

  const std::string name;
  Battery battery;
  EnConCurve en_con_curve;
};
