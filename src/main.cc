#include <driver_vehicle.hh>

#include <tools.hh>

#include <units.h>
using namespace units::literals;

using namespace units;
using namespace units::length;
using namespace units::energy;
using namespace units::velocity;
using namespace units::energy_consumption; // For iostream abbrv
using namespace units::energy_efficiency;
using namespace units::time;

#include <iostream>
#include <fstream>

int main()
{
  auto distance = 500_km;
  energy_consumption::Wh_per_km_t nrj_con(180_Wh / 1_km);
  std::cout << "Energy consumtion is " << nrj_con << " ("
    << energy_efficiency::km_per_kWh_t(1/nrj_con) << ", "
    << energy_efficiency::mi_per_kWh_t(1/nrj_con) << ", "
    << energy_efficiency::mi_per_gal_t(1/nrj_con)
    << ")" << std::endl;

  auto nrj_eff = energy_efficiency::mi_per_kWh_t(5.8);
  std::cout << "Energy efficiency " << nrj_eff << " ("
            << energy_consumption::Wh_per_km_t(1/nrj_eff) << ")" << std::endl;
  watt_hour_t energy = distance * nrj_con;

  std::cout << "energy needed for " << distance << " is " << energy << std::endl;

  charging_curve_key_points_t M3_cc;
  M3_cc.emplace_back(0, 25._kW);
  M3_cc.emplace_back(5, 190._kW);
  M3_cc.emplace_back(17, 190._kW);
  M3_cc.emplace_back(45, 150._kW);
  M3_cc.emplace_back(100, 2.0_kW);

  /*M3_cc.emplace_back(0, 75._kW);
  M3_cc.emplace_back(5, 250._kW);
  M3_cc.emplace_back(17, 250._kW);
  M3_cc.emplace_back(20, 200._kW);
  M3_cc.emplace_back(45, 150._kW);
  M3_cc.emplace_back(100, 2.0_kW);*/

  charging_curve_key_points_t MS_cc;
  MS_cc.emplace_back(0, 75._kW);
  MS_cc.emplace_back(5, 150._kW);
  MS_cc.emplace_back(45, 150._kW);
  MS_cc.emplace_back(100, 2.0_kW);

  charging_curve_key_points_t cc_up;
  cc_up.emplace_back(0, 10._kW);
  cc_up.emplace_back(100, 11._kW);

  charging_curve_key_points_t cc_down;
  cc_down.emplace_back(0, 11._kW);
  cc_down.emplace_back(100, 10._kW);

  charging_curve_key_points_t etron_cc;
  etron_cc.emplace_back(0, 148._kW);
  etron_cc.emplace_back(100, 150._kW);

  charging_curve_key_points_t leaf_cc;
  leaf_cc.emplace_back(0, 40._kW);
  leaf_cc.emplace_back(80, 45._kW);
  leaf_cc.emplace_back(100, 30._kW);
  Vehicle Nissan_Leaf_40kWh("Nissan_Leaf_40kWh", Battery(40._kWh, leaf_cc), 215.0_Whpkm);

  /*charging_curve_key_points_t gas_cc;
  gas_cc.emplace_back(0,   998._kW);
  gas_cc.emplace_back(100, 999._kW);
  Vehicle human("Human", Battery(100._kWh, gas_cc), 225.0_Whpkm);*/

  // Tesla
  Vehicle M3("Model 3", Battery(73_kWh, M3_cc), 185._Whpkm);
  Vehicle MS("Model S", Battery(92_kWh, MS_cc), 210._Whpkm);

  // Audi
  Vehicle eTron("E-tron", Battery(84_kWh, etron_cc), 310.0_Whpkm);

  // Nissan
  //Vehicle Nissan_Leaf_40kWh("Nissan_Leaf_40kWh", 40_kWh, etron_cc, 225.0_Whpkm);

  Vehicle infinite_range("Infinite range", Battery(999999._kWh, etron_cc), 225.0_Whpkm);
  Driver d("Normal driver");
  DriverVehicle dv_infinite(d, infinite_range);
	//std::vector dvs({dv_infinite});
	//std::vector<DriverVehicle> dvs({{d, infinite_range}});
	std::vector<DriverVehicle> dvs;
	//dvs.emplace_back({d, infinite_range});

  //std::vector vehicles({Nissan_Leaf_40kWh, M3, MS, eTron});
  std::vector vehicles({eTron});
	for (auto& v: vehicles)
		dvs.emplace_back(d, v);

  std::ofstream csv("time_vs_distance.csv");
  //auto& csv = std::cout;
  csv << "Car_name";
  auto distance_inc = 250._km;
  auto distance_min = distance_inc;
  auto distance_max = 350._km;
  //auto distance_min = 646._km;
  //auto distance_max = 648._km;
	std::cout << "Driver + Vehicle" << std::endl;
  for (auto d = distance_min; d <= distance_max; d += distance_inc)
    //csv << ", time_for_" << d;
    csv << ", " << d.value();
  csv << std::endl;
	for (DriverVehicle& dv: dvs) {
    csv << dv.name();
    for (auto d = distance_min; d <= distance_max; d += distance_inc) {
      time::minute_t duration = dv.time_to_do_trip(d, 150._km, 130._kph);
      auto average_speed = d / time::hour_t(duration);
      //csv << ", " << duration.value();
      csv << ", " << average_speed.value();
      //std::cout << "Drove " << d << " in " << tools::pretty_print(duration) <<std::endl << std::endl;
		}
    csv << std::endl;
	}
	std::cout << "Vehicle ONLY" << std::endl;
	for (const Vehicle& v: vehicles) {
    csv << v.name;
    for (auto d = distance_min; d <= distance_max; d += distance_inc) {
      time::minute_t duration = v.time_to_do_trip(d, 150._km, 130._kph);
      auto average_speed = d / time::hour_t(duration);
      //csv << ", " << duration.value();
      csv << ", " << average_speed.value();
      //std::cout << "Drove " << d << " in " << tools::pretty_print(duration) <<std::endl << std::endl;
    }
    csv << std::endl;
  }

  return 0;
}
