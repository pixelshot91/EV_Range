#include <charging_curve.hh>

using namespace units::power;

ChargingCurve::ChargingCurve(charging_curve_key_points_t key_points)
{
  if (key_points.front().soc != 0 || key_points.back().soc != 100)
    throw std::invalid_argument("Vector should start at 0%% and end at 100%%");

  auto prev_point = key_points.begin();
  auto next_point = key_points.begin() + 1;
  for (int soc = 0; soc <= 100; soc++) {
    std::cout << "soc = " << soc;
    std::cout << ", PREV = " << prev_point->soc;
    std::cout << ", NEXT = " << next_point->soc;
    if (next_point->soc < soc) {
      std::cout << "CHAGING POINT" << std::endl;
      prev_point++;
      next_point++;
    }
    auto power_incline = (next_point->power - prev_point->power) / (next_point->soc - prev_point->soc);
    points[soc] = prev_point->power + power_incline * (soc - prev_point->soc);
    std::cout << "  Power = " << points[soc] << std::endl;
  }
}
