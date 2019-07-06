#include <cars/tesla.hh>

#include <units.h>
using namespace units;
using namespace units::literals;

namespace cars {
  Vehicle Tesla::Model_3_Standard_Range() {
    charging_curve_key_points_t M3_cc;
    M3_cc.emplace_back(0, 75._kW);
    M3_cc.emplace_back(5, 250._kW);
    M3_cc.emplace_back(17, 250._kW);
    M3_cc.emplace_back(20, 200._kW);
    M3_cc.emplace_back(45, 150._kW);
    M3_cc.emplace_back(100, 2.0_kW);

    return Vehicle("Model 3 SR", 50_kWh, M3_cc, 180._Whpkm);
  }

  Vehicle Tesla::Model_3_Long_Range() {
    charging_curve_key_points_t M3_cc;
    M3_cc.emplace_back(0, 75._kW);
    M3_cc.emplace_back(5, 250._kW);
    M3_cc.emplace_back(17, 250._kW);
    M3_cc.emplace_back(20, 200._kW);
    M3_cc.emplace_back(45, 150._kW);
    M3_cc.emplace_back(100, 2.0_kW);

    return Vehicle("Model 3 LR", 75_kWh, M3_cc, 180._Whpkm);
  }

  Vehicle Tesla::Model_S_Long_Range() {
    charging_curve_key_points_t MS_cc;
    MS_cc.emplace_back(0, 75._kW);
    MS_cc.emplace_back(5, 150._kW);
    MS_cc.emplace_back(45, 150._kW);
    MS_cc.emplace_back(100, 2.0_kW);

    return Vehicle("Model S LR", 100_kWh, MS_cc, 200._Whpkm);
  }

}
