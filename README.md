# EVLDC - Electric Vehicle Long Distance Capability

## Introduction

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



```
s = "Python syntax highlighting"
print s
```

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

