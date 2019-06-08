#include <tools.hh>

#include <iomanip>
#include <sstream>

namespace tools
{
  std::string pretty_print(time::second_t t) {
    const unsigned h = t / time::hour_t(1);
    t -= time::hour_t(h);

    const int m = t / time::minute_t(1);
    t -= time::minute_t(m);

    const int s = t / time::second_t(1);

    std::stringstream ss;
    ss <<                                      h << ":"
       << std::setw(2) << std::setfill('0') << m << ":"
       << std::setw(2) << std::setfill('0') << s;

    return ss.str();
  }
}
