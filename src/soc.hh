#pragma once
#include <string>

class Soc {
public:
  explicit Soc(double v);
  //operator double() const { return value; }

  void set(double v);
  void set_from_percentage(int p);

  double as_ratio() const;
	int as_percentage() const;
	std::string str() const;

private:
  void check();
	double value;
};

std::ostream& operator<<(std::ostream& stream, const Soc& soc);

inline bool operator==(const Soc& lhs, const Soc& rhs){ return lhs.as_ratio() == rhs.as_ratio(); }
inline bool operator!=(const Soc& lhs, const Soc& rhs){return !operator==(lhs,rhs);}
inline bool operator< (const Soc& lhs, const Soc& rhs){ return lhs.as_ratio() < rhs.as_ratio(); }
inline bool operator> (const Soc& lhs, const Soc& rhs){return  operator< (rhs,lhs);}
inline bool operator<=(const Soc& lhs, const Soc& rhs){return !operator> (lhs,rhs);}
inline bool operator>=(const Soc& lhs, const Soc& rhs){return !operator< (lhs,rhs);}
