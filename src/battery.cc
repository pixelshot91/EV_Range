#include <battery.hh>

using namespace units::literals;
using namespace units::power;
Battery::Battery(energy::kilowatt_hour_t capacity, ChargingCurve cc)
  : capacity(capacity),
		energy(capacity),
    maximum_cc(cc)
{
  //cc_map.insert({power::kilowatt_t(NAN), cc});
}

void Battery::set_to_full() {
	energy = capacity;
}

void Battery::discharge(energy::kilowatt_hour_t e) {
	if (e > energy)
		throw std::invalid_argument("Cant remove so much energy from battery");
	energy -= e;
}

void Battery::charge_for(time::minute_t duration, power::kilowatt_t max_charger_power) {
	if (energy > capacity)
		throw std::invalid_argument("Battery more than 100%% before charging");
	energy = get_cc(max_charger_power).energy_gained_from_charging(energy/capacity, duration, capacity);
	if (energy > capacity)
		throw std::invalid_argument("Battery more than 100%% after charging");
}

time::minute_t Battery::charge_to(Soc soc_goal, power::kilowatt_t max_charger_power) {
	auto time = get_time_to_recharge(soc_interval(soc(), soc_goal), max_charger_power);
	energy = capacity * soc_goal.as_ratio();
	return time;
}

Soc Battery::soc() const {
	return Soc(energy / capacity);
}

const ChargingCurve& Battery::get_cc(power::kilowatt_t max_charger_power) const
{
	if (max_charger_power >= maximum_cc.peak_power)
		return maximum_cc;
  //std::map<double, ChargingCurve> m;
  ChargingCurve low_cc = maximum_cc.min(max_charger_power);
  //m.insert(std::pair(12, low_cc));
  if (!cc_map.count(max_charger_power)) {

    cc_map.insert(std::pair(max_charger_power, low_cc));
  }
  auto it = cc_map.find(max_charger_power);
  if (it == cc_map.end()) {
    throw std::invalid_argument("Impossible, cc not found");
  }
  const ChargingCurve& cc = it->second;
  return cc;
}

soc_interval Battery::get_soc_interval_for(const energy::watt_hour_t energy_to_gain, const power::kilowatt_t max_charger_power) const
{
  return get_cc(max_charger_power).get_soc_interval_for(energy_to_gain, capacity);
}

time::minute_t Battery::get_time_to_recharge(const soc_interval si, const power::kilowatt_t max_charger_power) const {
  return get_cc(max_charger_power).get_time_to_recharge_soc(si, capacity);
}

time::minute_t Battery::get_time_to_recharge(const energy::watt_hour_t energy_to_gain, const Soc minimum_soc, const power::kilowatt_t max_charger_power) const
{
  auto si = get_soc_interval_for(energy_to_gain, max_charger_power);
  if (si.low.as_ratio() < minimum_soc.as_ratio()) {
    std::cout << "re adjusting soc" << std::endl;
    si.high.set(si.high.as_ratio() + minimum_soc.as_ratio() - si.low.as_ratio());
    si.low.set(minimum_soc.as_ratio());
  }
  return get_time_to_recharge(si, max_charger_power);
}
