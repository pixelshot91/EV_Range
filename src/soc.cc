#include <soc.hh>

#include <stdexcept>

void Soc::check() {
  const double epsilon = 1E-3;
  if (value < 0 || 1.0 + epsilon < value)
    throw std::invalid_argument("Soc not betwwen 0 and 1. Soc is " + std::to_string(value));
  if (value > 1.0)
    value = 1.0;
}

Soc::Soc(double v)
  : value(v)
{
  check();
}

void Soc::set(double v) {
  check();
  value = v;
}

void Soc::set_from_percentage(int p) {
  set(p / 100.0);
}

double Soc::as_ratio() const {
  return value;
}

int Soc::as_percentage() const {
	return value * 100;
}

std::string Soc::str() const {
	return std::to_string(as_percentage()) + "%";
}

std::ostream& operator<<(std::ostream& stream, const Soc& soc) {
    stream << soc.str();
    return stream;
 }
