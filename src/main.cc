#include <vehicle.hh>

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
  M3_cc.emplace_back(0, 75._kW);
  M3_cc.emplace_back(5, 250._kW);
  M3_cc.emplace_back(17, 250._kW);
  M3_cc.emplace_back(20, 200._kW);
  M3_cc.emplace_back(45, 150._kW);
  M3_cc.emplace_back(100, 2.0_kW);

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
  Vehicle Nissan_Leaf_40kWh("Nissan_Leaf_40kWh", 40_kWh, leaf_cc, 215.0_Whpkm);

  Vehicle infinite_range("Infinite range", 999999_kWh, etron_cc, 225.0_Whpkm);

  charging_curve_key_points_t gas_cc;
  gas_cc.emplace_back(0,   998._kW);
  gas_cc.emplace_back(100, 999._kW);
  Vehicle human("Human", 100._kWh, gas_cc, 225.0_Whpkm);

  // Tesla
  Vehicle M3("Model 3", 73_kWh, M3_cc, 200._Whpkm);
  Vehicle MS("Model S", 92_kWh, MS_cc, 210._Whpkm);

  // Audi
  Vehicle eTron("E-tron", 84_kWh, etron_cc, 250.0_Whpkm);

  // Nissan
  //Vehicle Nissan_Leaf_40kWh("Nissan_Leaf_40kWh", 40_kWh, etron_cc, 225.0_Whpkm);

  std::vector vehicles({Nissan_Leaf_40kWh, M3, MS, eTron, infinite_range, human});
  //std::vector vehicles({human});
  /*for (const Vehicle& v: {M3, MS}) {
    auto distance = 500._km;
    auto time = v.time_to_do_trip(distance, 200._km, 130._kph);
    std::cout << v.name << " -> " << tools::pretty_print(time) << "; ";
  }
  return 0;*/

  /*for (const Vehicle& v: {M3, eTron}) {
    std::cout << " === " << v.name << " ===" << std::endl;
    auto charging_time = v.get_time_to_recharge(distance, 150._kW);
    std::cout << "Optimal charging time for " << v << " to get " << distance << " is " << tools::pretty_print(charging_time) << std::endl;
  }*/

  //M3.get_time_for_distance(500._km, 10._km, 20._km, 130._kph);

  /*for (auto distance = 300._km; distance < 1000._km; distance += 100._km) {
    std::cout << "distance = " << distance << ": ";
    for (const Vehicle& v: vehicles) {
      std::cout << " == " << v.name << " == " << std::endl;
      auto time = v.time_to_do_trip(distance, 200._km, 130._kph);
      std::cout << v.name << " -> " << tools::pretty_print(time) << "; ";
      std::cout << std::endl;
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }*/
  std::ofstream csv("time_vs_distance.csv");
  //auto& csv = std::cout;
  csv << "Car_name";
  auto distance_min = 000._km;
  auto distance_max = 1500._km;
  //auto distance_min = 646._km;
  //auto distance_max = 648._km;
  auto distance_inc = 1._km;
  for (auto d = distance_min; d <= distance_max; d += distance_inc)
    csv << ", time_for_" << d;
  csv << std::endl;
  for (const Vehicle& v: vehicles) {
    csv << v.name;
    for (auto d = distance_min; d <= distance_max; d += distance_inc) {
      time::minute_t duration = v.time_to_do_trip(d, 150._km, 130._kph);
      auto average_speed = d / time::hour_t(duration);
      //csv << ", " << duration.value();
      csv << ", " << average_speed.value();
      std::cout << "Drove " << d << " in " << tools::pretty_print(duration) <<std::endl << std::endl;
    }
    csv << std::endl;
  }

  return 0;
}
