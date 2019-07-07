#include <charging_curve.hh>

#include <algorithm>

using namespace units::literals;
using namespace units::power;
using namespace units::energy;
using namespace units::time;

void ChargingCurve::init_soc_intervals()
{
  auto max = std::max_element(points.rbegin() + 1, points.rend());
  auto best_soc = std::distance(points.begin(), max.base() - 1);


  soc_intervals[0].low = best_soc;
  soc_intervals[0].high = best_soc;

  auto low_soc = best_soc;
  auto high_soc = best_soc + 1;

  for (int soc_gain = 1; soc_gain <= 100; soc_gain++) {
    soc_intervals[soc_gain].low = low_soc;
    soc_intervals[soc_gain].high = high_soc;
    //std::cout << soc_gain << ": " << low_soc << " --> " << high_soc << std::endl;
    if (high_soc < 100 && (points[low_soc - 1] < points[high_soc] || low_soc < 1))
      high_soc++;
    else
      low_soc--;
  }
}

ChargingCurve::ChargingCurve(charging_curve_key_points_t key_points)
{
  if (key_points.size() == 0 || key_points.front().soc != 0 || key_points.back().soc != 100)
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
  std::array<power::kilowatt_t, 101> pts;
  for (int soc = 0; soc <= 100; soc++)
    pts[soc] = std::min(this->points[soc], max_charger_output);
  return ChargingCurve(pts);
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
  const auto& soc_interval = soc_intervals.at(soc_to_gain);
  for (int soc = soc_interval.low; soc < soc_interval.high; soc++) {
    auto time_1_percent = percent_soc / points[soc];
    std::cout << "point[soc = " << soc << "] = " << points[soc] << std::endl;
    //std::cout << time::second_t(time_1_percent).value() << std::endl;
    charging_time += time_1_percent;
    //std::cout << "time_1_percent " << soc << "= " << time_1_percent << std::endl;
  }
  //std::cout << "energy needed = " << energy_to_gain << std::endl;
  //std::cout << "energy_gained = " << (soc_interval.high - soc_interval.low) * percent_soc << std::endl;
  std::cout << "So about 1% every " << charging_time.value() / soc_to_gain << std::endl;
  std::cout << "charging_time  = " << charging_time << std::endl;
  return charging_time;
}

time::minute_t ChargingCurve::get_time_to_recharge_soc(soc_interval soc_interval, energy::watt_hour_t battery_capacity) const
{
  /*for (int i = 0; i <= 100; i++)
    std::cout << "p[" << i << "] = " << points[i] << std::endl;*/

  auto percent_soc = battery_capacity / 100;
  auto charging_time = 0.0_s;
  for (int soc = soc_interval.low; soc < soc_interval.high; soc++) {
    auto time_1_percent = percent_soc / points[soc];
    charging_time += time_1_percent;
    //std::cout << "time_1_percent " << soc << "= " << time_1_percent << std::endl;
  }
  //std::cout << "energy needed = " << energy_to_gain << std::endl;
  //std::cout << "energy_gained = " << (soc_interval.high - soc_interval.low) * percent_soc << std::endl;
  std::cout << "So about 1% every " << charging_time.value() / (soc_interval.high - soc_interval.low) << std::endl;
  return charging_time;
}


soc_interval ChargingCurve::get_soc_interval_for(const energy::watt_hour_t energy_to_gain, energy::watt_hour_t battery_capacity) const {
  double soc_to_gain = energy_to_gain / battery_capacity * 100;
  return soc_intervals.at(soc_to_gain);
}
