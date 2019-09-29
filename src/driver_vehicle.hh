#pragma once

#include <driver.hh>
#include <vehicle.hh>

class DriverVehicle {
public:
  DriverVehicle(Driver d, Vehicle v);
	std::string name() const;
	time::second_t time_to_do_trip(const length::kilometer_t distance_goal, const length::kilometer_t distance_between_charger, const velocity::kilometers_per_hour_t cruising_speed);

//private:
  Driver driver;
  Vehicle vehicle;
};
