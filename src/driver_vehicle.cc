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
	const bool debug = false;

	auto charger_detour = 5._min;
	auto max_charger_power = 35099._kW;
	const Soc minimum_soc(0.10);


	Battery& battery = vehicle.battery;
	auto en_con = vehicle.en_con_curve.get_consumption_at_speed(cruising_speed);
	length::kilometer_t range = vehicle.battery.capacity / en_con;

	if (debug) std::cout << "\ndistance goal = " << length::kilometer_t(distance_goal) << ", range = " << range << std::endl;

	auto soc_interval = battery.get_soc_interval_for(distance_between_charger * en_con, max_charger_power);
	if (debug) std::cout << "soc_interval low=" << soc_interval.low << ", high=" << soc_interval.high << std::endl;
	//double low_soc = std::max(soc_interval.low / 100.0, minimum_soc);
	//double low_soc = minimum_soc;
	//if (debug) std::cout << "low_soc = " << low_soc << std::endl;

	battery.set_to_full();
	driver.rest();
	auto total_time = 0._min;
	auto total_distance_driven = 0._km;
	const auto reserve_energy = battery.capacity * minimum_soc.as_ratio();

	while (total_distance_driven < distance_goal) {
		length::kilometer_t d1 = (battery.energy - reserve_energy) / en_con;
		length::kilometer_t distance_remaining = distance_goal - total_distance_driven;

		//length::kilometer_t distance_driven = (battery.energy - reserve_energy) / en_con;
		auto distance_driven = std::min(d1, distance_remaining);

		auto driving_time = distance_driven / cruising_speed;
		if (debug) std::cout << "Vehicle can drive " << distance_driven << " in " << tools::pretty_print(driving_time) << std::endl;
		bool pause_needed = false;
		if (driver.time_before_pause() < driving_time) { // Stop first for driver
			driving_time = driver.time_before_pause();
			distance_driven = driving_time * cruising_speed;
			if (debug) std::cout << "But driver can only drive for " << tools::pretty_print(driving_time) << " (" << distance_driven << ")" << std::endl;
			pause_needed = true;
		}

		// Driving
		battery.discharge(distance_driven * en_con);
		total_time += driving_time;
		driver.drive_for(driving_time);
		total_distance_driven += distance_driven;

		distance_remaining = distance_goal - total_distance_driven;

		if (total_distance_driven >= distance_goal)
			break;

		if (driver.time_before_pause() < 15._min)
			pause_needed = true;

		// Charging + Pause
		if (pause_needed) {
			auto pause_duration = driver.take_a_pause();
			if (debug) std::cout << "pause duration " << tools::pretty_print(pause_duration) << std::endl;
			battery.charge_for(pause_duration, max_charger_power);
			total_time += charger_detour + pause_duration;
		} else {
			if (debug) std::cout << "Charging only" << std::endl;
			auto energy_need_for_rest_of_trip = distance_remaining * en_con + reserve_energy;
			double soc_needed(energy_need_for_rest_of_trip / battery.capacity);
			//Soc high_soc = std::min(soc_interval.high, soc_needed);
			Soc high_soc = soc_interval.high;
			if (soc_needed < high_soc.as_ratio())
				high_soc.set(soc_needed);
			if (debug) std::cout << "Charging from " << battery.soc() << " -> " << high_soc << std::endl;
			auto charging_time = battery.charge_to(high_soc, max_charger_power);
			if (debug) std::cout << "Charging time = " << tools::pretty_print(charging_time) << std::endl;
			if (charging_time > driver.next_pause_duration()) {
				if (debug) std::cout << "Driver might as well take a pause" << std::endl;
				driver.take_a_pause();
			}
			total_time += charger_detour + charging_time;
		}
	}
	return total_time;
}

