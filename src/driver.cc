#include <driver.hh>

#include <tools.hh>

Driver::Driver(std::string name)
	: name(name)
{
  pauses.emplace_back(2._h, 15._min);
  pauses.emplace_back(3._h, 45._min);
  pauses.emplace_back(2._h, 15._min);
  it = pauses.begin();
}

void Driver::rest() {
	it = pauses.begin();
	driving_time = 0._min;
}

void Driver::drive_for(const time::minute_t time) {
  const Pause& p = *it;
  driving_time += time;
  if (driving_time > p.interval)
    throw std::invalid_argument("Pause overrun. Driving for " + tools::pretty_print(driving_time) + " but driver can only drive for " + tools::pretty_print(p.interval));
}

time::minute_t Driver::take_a_pause() {
	//std::cout << "DRIVER TAKE A PAUSE" << std::endl;
  auto duration = it->duration;
  it++;
	if (it == pauses.end())
		it = pauses.begin();
  driving_time = 0._min;
  return duration;
}

time::minute_t Driver::time_before_pause() const
{
	if (it == pauses.end())
		throw std::invalid_argument("no more pause");
	//std::cout << "time before pause = " << tools::pretty_print(it->interval - driving_time);
  return it->interval - driving_time;
}

time::minute_t Driver::next_pause_duration() const {
	return it->duration;
}
