#pragma once

#include <driver.hh>
#include <vehicle.hh>

class DriverVehicle {
public:
  DriverVehicle(Driver d, Vehicle v);

private:
  Driver driver;
  Vehicle vehicle;
};
