#include <charging_curve.hh>

#include <algorithm>

using namespace units::literals;
using namespace units::power;
using namespace units::time;

void ChargingCurve::init_soc_intervals()
{
  auto max = std::max_element(points.rbegin(), points.rend());
  auto best_soc = std::distance(points.begin(), max.base() - 1);

  auto low_soc = best_soc;
  auto high_soc = best_soc + 1;

  for (int soc_gain = 1; soc_gain < 100; soc_gain++) {
    soc_intervals[soc_gain].low = low_soc;
    soc_intervals[soc_gain].high = high_soc;
    std::cout << soc_gain << ": " << low_soc << " -> " << high_soc << std::endl;
    if (points[low_soc - 1] < points[high_soc] || low_soc <= 1)
      high_soc++;
    else
      low_soc--;
  }
}

ChargingCurve::ChargingCurve(charging_curve_key_points_t key_points)
{
  if (key_points.front().soc != 0 || key_points.back().soc != 100)
    throw std::invalid_argument("Vector should start at 0%% and end at 100%%");

  auto prev_point = key_points.begin();
  auto next_point = key_points.begin() + 1;
  for (int soc = 0; soc <= 100; soc++) {
    //std::cout << "soc = " << soc;
    //std::cout << ", PREV = " << prev_point->soc;
    //std::cout << ", NEXT = " << next_point->soc;
    if (next_point->soc < soc) {
      //std::cout << "CHAGING POINT" << std::endl;
      prev_point++;
      next_point++;
    }
    auto power_incline = (next_point->power - prev_point->power) / (next_point->soc - prev_point->soc);
    points[soc] = prev_point->power + power_incline * (soc - prev_point->soc);
    //std::cout << "  Power = " << points[soc] << std::endl;
  }

  init_soc_intervals();
}

ChargingCurve::ChargingCurve(std::array<power::kilowatt_t, 101> points)
  : points(points)
{
  init_soc_intervals();
}

ChargingCurve ChargingCurve::min(const power::kilowatt_t max_charger_output) const
{
  std::array<power::kilowatt_t, 101> points;
  for (int soc = 0; soc <= 100; soc++)
    points[soc] = std::min(this->points[soc], max_charger_output);
  return ChargingCurve(points);
}

std::array<power::kilowatt_t, 101>::iterator make_forward(std::array<power::kilowatt_t, 101>::reverse_iterator rit)
{
  return rit.base() - 1;
}

//time::minute_t ChargingCurve::get_time_to_recharge(double soc_gain)
time::minute_t ChargingCurve::get_time_to_recharge(energy::watt_hour_t energy_to_gain, energy::watt_hour_t battery_capacity)
{
  /*for (int i = 0; i <= 100; i++)
    std::cout << "p[" << i << "] = " << points[i] << std::endl;*/

  double soc_to_gain = energy_to_gain / battery_capacity * 100;
  std::cout << "soc to gain = " << soc_to_gain << std::endl;
  auto percent_soc = battery_capacity / 100;
  auto charging_time = 0.0_s;
  const auto& soc_interval = soc_intervals[soc_to_gain];
  for (double soc = soc_interval.low; soc < soc_interval.high; soc++) {
    auto time_1_percent = percent_soc / points[soc];
    charging_time += time_1_percent;
    std::cout << "time_1_percent " << soc << "= " << time_1_percent << std::endl;
  }

  return charging_time;
}
