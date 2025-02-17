CXX=g++
CXXFLAGS = -std=c++17 -Isrc -g -Wall -Wextra
SRC := main.cc vehicle.cc charging_curve.cc tools.cc battery.cc driver.cc driver_vehicle.cc soc.cc
SRC := $(addprefix src/, $(SRC))
OBJ := $(subst .cc,.o,$(SRC))
BIN := ev_range

.PHONY: all
all: $(BIN)

#DEPS: $(OBJ:.o=.d)
#
#-include $(DEPS)
#
#%.o: %.cc
#	$(CXX) $(CXXFLAGS) -MMD -MF $(patsubst %.o,%.d,$@) -o $@ $<

%.o: %.cc %.hh
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $(OBJ)

.PHONY: clean
clean:
	rm -f $(OBJ)
