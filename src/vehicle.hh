#pragma once
#include <charging_curve.hh>

#include "units.h"

using namespace units;

class Vehicle {
public:
  Vehicle(std::string name, energy::kilowatt_hour_t battery_capacity, charging_curve_key_points_t key_points);

  const std::string name;
  const energy::kilowatt_hour_t battery_capacity;
  ChargingCurve max_power_charging_curve;
};
