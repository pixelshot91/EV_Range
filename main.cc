//#define UNIT_LIB_DISABLE_IOSTREAM
#include "units.h"
using namespace units::literals;

using namespace units;
using namespace units::length;
using namespace units::energy;
using namespace units::velocity;
using namespace units::frequency;
using namespace units::energy_consumption; // For iostream abbrv
using namespace units::energy_efficiency;

//using acceleration = compound_unit<meters, inverse<squared<seconds>>>;

/*namespace units::detail {
	UNIT_ADD(units::velocity, kilometers_per_sec, kilometers_per_sec, kmpsec, compound_unit<length::kilometers, inverse<time::seconds>>)
}*/
//UNIT_ADD(velocity, kilometer_per_sec, kilometer_per_sec, kmpsec, compound_unit<length::kilometer, inverse<time::seconds>>)

//UNIT_ADD(energy_efficiency, Wh_per_km, Wh_per_km, Whpkm, compound_unit<energy::watt_hour, inverse<length::kilometer>>)
//UNIT_ADD_CATEGORY_TRAIT(energy_efficiency)
//UNIT_ADD(energy_efficiency, miles_per_kWh, miles_per_kWh, mpkWh, compound_unit(<length::mile, inverse<energy::kilowatt_hour>>)

#include <iostream>

int main()
{
  units::length::meter_t a = 2_m;
  units::length::meter_t b = 5_m;
  auto area = a * b;
  std::cout << "Test = " << units::length::foot_t(a) << std::endl;
  units::length::foot_t foot_len(a);
  std::cout << "Test = " << foot_len << std::endl;
  std::cout << "Area = " << area << std::endl;


  time::second_t myperiod = 2_s;
  frequency::hertz_t f = 1 / myperiod;
  std::cout << "f = " << f << std::endl;
  //frequency::hertz f = dimensionless::dimensionless_t(1) / myperiod;
//  frequency::hertz f = units::math::pow<-1>(myperiod);
//  frequency::hertz f = units::math::pow<-1>(myperiod);

  auto f1 = a / myperiod / a;
  auto f2 = 1 / myperiod;
  //frequency::hertz ff1 = f1;
  std::cout << "f1= " << f1 << std::endl;
  std::cout << "f2= " << f2 << std::endl;

  frequency::hertz_t fg = 2_Hz;
  std::cout << "fg= " << fg << std::endl;
  //std::cout << "fg= " << to_string(fg) << std::endl;

  //units::velocity::kilometers_per_hour_t v = 60_kph;
  //units::velocity::kilometers_per_hour_t v = 100_mps;
  auto v1 = 100_kph;
  auto v2 = 60_mph;
  auto diff = v1 - v2;
  std::cout << "Velocity = " << diff << std::endl;
  //velocity::feet_per_second feet(diff);
  //velocity::feet_per_second feet(diff);
  //velocity::meters_per_second_t feet(diff);
  //std::cout << "Velocity = " << velocity::feet_per_second(diff) << std::endl;

  kilometers_per_sec_t v3 = 0.1_kmpsec;
  std::cout << "V3 = " << v3 << " = " << kilometers_per_hour_t(v3) << std::endl;

  kilometers_per_sec_t v4(0.1);
  std::cout << "V4 = " << v4 << " = " << kilometers_per_hour_t(v4) << std::endl;

  auto v5 = 0.1_kmpsec;
  std::cout << "V5 = " << v5 << " = " << kilometers_per_hour_t(v5) << std::endl;

  std::string v5s = to_string(v5);
  std::cout << "v5s " << v5s << std::endl;

  auto distance = 300_km;
  //km d1(2);
  energy_consumption::Wh_per_km_t nrj_con(180_Wh / 1_km);
  //std::cout << "Energy consumtion is " <<  to_string(nrj_con) << " (" << energy_efficiency::km_per_Wh_t(1/nrj_con) << ")" << std::endl;
  //std::cout << "Energy consumtion is " <<  energy_consumption::Wh_per_km_t(nrj_con) << " (" << energy_efficiency::km_per_kWh_t(1/nrj_con) << ")" << std::endl;
  std::cout << "Energy consumtion is " << nrj_con << " ("
    << energy_efficiency::km_per_kWh_t(1/nrj_con) << ", "
    << energy_efficiency::mi_per_kWh_t(1/nrj_con) << ", "
    << energy_efficiency::mi_per_gal_t(1/nrj_con)
    << ")" << std::endl;
  watt_hour_t energy = distance * nrj_con;

  auto c1_ef = 12_mpg;
  auto c2_ef = 15_mpg;

  std::cout << energy_consumption::Wh_per_km_t(1/c1_ef) << " , " << energy_consumption::Wh_per_km_t(1/c2_ef) << std::endl;

  std::cout << "energy needed for " << distance << " is " << energy << std::endl;
  return 0;
}
