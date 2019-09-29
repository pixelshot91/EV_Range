#include <driver_vehicle.hh>

#include <tools.hh>
using namespace units::length;

DriverVehicle::DriverVehicle(Driver d, Vehicle v)
	: driver(d)
	, vehicle(v)
{}

std::string DriverVehicle::name() const {
	return vehicle.name + " driven by " + driver.name;
}

time::second_t DriverVehicle::time_to_do_trip(const length::kilometer_t distance_goal, const length::kilometer_t distance_between_charger, const velocity::kilometers_per_hour_t cruising_speed)
{
	const bool debug = true;
	
	auto charger_detour = 5._min;
	auto max_charger_power = 35099._kW;
	const double minimum_soc = 0.10;

	
	Battery& battery = vehicle.battery;
	auto en_con = vehicle.en_con_curve.get_consumption_at_speed(cruising_speed);
	if (debug) std::cout << "en_con " << en_con << std::endl;
	length::kilometer_t range = vehicle.battery.capacity / en_con;
	
	if (debug) std::cout << "distance goal = " << length::kilometer_t(distance_goal) << ", range = " << range << std::endl;
	
	auto soc_interval = battery.get_soc_interval_for(distance_between_charger * en_con, max_charger_power);
	//double low_soc = std::max(soc_interval.low / 100.0, minimum_soc);
	double low_soc = minimum_soc;
	if (debug) std::cout << "low_soc = " << low_soc << std::endl;

	battery.set_to_full();
	driver.rest();
	auto total_time = 0._min;
	auto total_distance_driven = 0._km;
	const auto reserve_energy = battery.capacity * low_soc;

	while (total_distance_driven < distance_goal) {
		if (debug) std::cout << "available energy = " << (battery.energy - reserve_energy) << battery.energy << reserve_energy << std::endl;
		//auto distance_driven = std::min((battery.energy - reserve_energy) / en_con, distance_goal - total_distance_driven);
		//length::kilometer_t distance_driven = std::min((battery.energy - reserve_energy) / en_con, distance_goal - total_distance_driven);
		length::kilometer_t d1 = (battery.energy - reserve_energy) / en_con;
		length::kilometer_t d2 = distance_goal - total_distance_driven;

		//length::kilometer_t distance_driven = (battery.energy - reserve_energy) / en_con;
		auto distance_driven = std::min(d1, d2);
		
		auto driving_time = distance_driven / cruising_speed;
		if (debug) std::cout << "Vehicle can drive " << distance_driven << " in " << tools::pretty_print(driving_time) << std::endl;
		if (driver.time_before_pause() < driving_time) { // Stop first for driver
			driving_time = driver.time_before_pause();
			distance_driven = driving_time * cruising_speed;
			if (debug) std::cout << "But driver can only drive for " << tools::pretty_print(driving_time) << " (" << distance_driven << ")" << std::endl;
		}

		// Driving
		battery.discharge(distance_driven * en_con);
		total_time += driving_time;
		total_distance_driven += distance_driven;

		if (total_distance_driven >= distance_goal)
			break;
		
		// Charging + Pause
		auto pause_duration = driver.take_a_pause();
		if (debug) std::cout << "pause duration " << tools::pretty_print(pause_duration) << std::endl;
		battery.charge_for(pause_duration, max_charger_power);
		total_time += charger_detour + pause_duration;
	}
	return total_time;
}

