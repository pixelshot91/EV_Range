#include <vehicle.hh>

#include <units.h>
using namespace units::literals;

using namespace units;
using namespace units::length;
using namespace units::energy;
using namespace units::velocity;
using namespace units::energy_consumption; // For iostream abbrv
using namespace units::energy_efficiency;

#include <iostream>

int main()
{
  auto distance = 300_km;
  energy_consumption::Wh_per_km_t nrj_con(180_Wh / 1_km);
  std::cout << "Energy consumtion is " << nrj_con << " ("
    << energy_efficiency::km_per_kWh_t(1/nrj_con) << ", "
    << energy_efficiency::mi_per_kWh_t(1/nrj_con) << ", "
    << energy_efficiency::mi_per_gal_t(1/nrj_con)
    << ")" << std::endl;
  watt_hour_t energy = distance * nrj_con;

  std::cout << "energy needed for " << distance << " is " << energy << std::endl;

  charging_curve_key_points_t cc;
  cc.emplace_back(0, 15.0_kW);
  cc.emplace_back(10, 250_kW);
  cc.emplace_back(20, 250_kW);
  cc.emplace_back(100, 2.0_kW);

  Vehicle M3("Model 3", 75_kWh, cc);

  return 0;
}
