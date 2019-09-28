# EVLDC - Electric Vehicle Long Distance Capability

## Introduction
EVLDC enables quick and easy visual comparison of various EV Long Distance Capabilities.
It shows in a graph the average speed of a vehicle as a function of distance for a set cruising speed, taking into account the charging time and the rest time of the driver.
The average speed will be slightly lower than the cruising speed for an fast recharging car (e.g. an ICE car) and will drop significantly for an EV after the initial range has been exhausted.
An EV which can charge with a lot of power for a long time (the area under the charging curve is great), and which is efficient (the energy consummed by unit of distance is low) will charge faster and will lose less time at charging stop, thus increasing the overall average speed.

## Setup
```
sudo apt install python3-pip
pip3 install matplotlib
pip3 install scipy
```
## Build
```
make
./ev_range
python3 python_graph/main.py
```

## Dependencies
### C++ Units
From https://github.com/nholthaus/units.git, a compile-time header-only c++14 unit library. It statically ensures dimensional homogeneity at each calculation.

### YAML
The YAML format will be used to store data about car like name, battery capacity and charging curve.
https://github.com/jbeder/yaml-cpp.git (not used yet)

## Chart library

| Language | Name        | interactive zoom                              | color channel |
| -------- | ----------  | --------------------------------------------- | ------------- |
| Python   | matplotlib  | select magnif. mouse. Backscape for last zoom | No            |
| C++      | QCustomPlot |                                               | Yes           |

Chart option

  * Python
      * pyplot  
           \+ zoom, slider  
           \- no color channel
  * C++:
      * matplotlib: same as pyplot
      * QCustomPlot: old, color channel
      * QChart  
          \+ official, color channel (AreaChart), smooth zoom, click on legend to hide/show (legendMarker), fast (OpenGL series)  
          \- closed source


### QCustomPlot Installation  
  GO to https://www.qcustomplot.com/index.php/download and download QCustomPlot.tar.gz.
```
tar xf QCustomPlot.tar.gz
cd qcustomplot/examples/plots
qmake
make
./plot-examples
```
  You will see the quadratic example.
  Change number in mainwindow.cpp "setupDemo(X);"

