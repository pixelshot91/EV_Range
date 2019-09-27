#include <driver.hh>

Driver::Driver()
{
  pauses.emplace_back(2._h, 15._min);
  pauses.emplace_back(2._h, 45._min);
  it = pauses.begin();
}

time::minute_t Driver::time_to_next_pause() const
{
  return 13._min;
}

void Driver::drive_for(const time::minute_t time) {
  const Pause& p = *it;
  driving_time += time;
  if (p.interval > driving_time)
    std::cout << "Pause overrun" << std::endl;
}

time::minute_t Driver::take_a_pause() {
  auto duration = it->duration;
  it++;
  driving_time = 0._min;
  return duration;
}

time::minute_t Driver::time_before_pause() const
{
  return it->interval - driving_time;
}
