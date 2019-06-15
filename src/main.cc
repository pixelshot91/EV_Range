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

int main()
{
  auto distance = 800_km;
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

  charging_curve_key_points_t cc;
  cc.emplace_back(0, 75._kW);
  cc.emplace_back(5, 250._kW);
  cc.emplace_back(17, 250._kW);
  cc.emplace_back(20, 200._kW);
  cc.emplace_back(45, 150._kW);
  cc.emplace_back(100, 2.0_kW);

  charging_curve_key_points_t cc_up;
  cc_up.emplace_back(0, 10._kW);
  cc_up.emplace_back(100, 11._kW);

  charging_curve_key_points_t cc_down;
  cc_down.emplace_back(0, 11._kW);
  cc_down.emplace_back(100, 10._kW);

  charging_curve_key_points_t etron_cc;
  etron_cc.emplace_back(0, 150._kW);
  etron_cc.emplace_back(100, 150._kW);

  Vehicle M3("Model 3", 75_kWh, cc, 180._Whpkm);
  Vehicle eTron("E-tron", 90_kWh, etron_cc, 225.0_Whpkm);

  /*for (const Vehicle& v: {M3, eTron}) {
    std::cout << " === " << v.name << " ===" << std::endl;
    auto charging_time = v.get_time_to_recharge(distance, 150._kW);
    std::cout << "Optimal charging time for " << v << " to get " << distance << " is " << tools::pretty_print(charging_time) << std::endl;
  }*/

  //M3.get_time_for_distance(500._km, 10._km, 20._km, 130._kph);
  auto time = M3.time_to_do_trip(distance, 200._km, 130._kph);
  std::cout << "distance = " << distance << " -> " << tools::pretty_print(time) << std::endl;

  return 0;
}
