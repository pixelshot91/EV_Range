#!/usr/bin/env python
import csv
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import make_interp_spline, BSpline
from scipy.ndimage.filters import gaussian_filter1d
import datetime

with open('time_vs_distance.csv') as csv_file:
    #csv_reader = csv.DictReader(csv_file)
    #line_count = 0
    #for row in csv_reader:
    #    if line_count == 0:
    #        print(f'Column names are {", ".join(row)}')
    #        line_count += 1
    #    print(row)
    #    #print(f'\t{row["Car_name"]} take row["100km"] for 100_kworks in the {row["department"]} department, and was born in {row["birthday month"]}.')
    #    line_count += 1
    #print(f'Processed {line_count} lines.')
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        if line_count == 0:
            #print(f'Column names are {", ".join(row)}')
            #print("max = " + row[-1].split(' ')[1].split('_')[2])
            x = [int(n) for n in row[1:]]
            #max=int(row[-1].split(' ')[1].split('_')[2])
            #xaxis = [n for n in range(1, max)]
            #T = np.array(xaxis)
            #print("T.len = ", len(T))
            #xnew = np.linspace(1,max,50) #300 represents number of points to make between T.min and T.max
            line_count += 1

        else:
            #print(row)
            print(row[0])
            numeric = [float(t) for t in row[1:]]
            #plt.plot(numeric, label=row[0])

            # Spline
            #power = np.array(row[1:])
            #spl = make_interp_spline(T, power, k=3) #BSpline object
            #power_smooth = spl(xnew)

            #plt.plot(xnew, power_smooth, label=row[0] + "_spline")

            # Gaussian
            ysmoothed = gaussian_filter1d(numeric, sigma=1.2)
            plt.plot(x, ysmoothed, label=row[0])
            line_count += 1
    print(f'Processed {line_count} lines.')

    #plt.plot(400, 115, 'ro', marker='$c$', label='challenge')
    x = 1000
    y = 80
    offset = 0.3
    #plt.scatter(x, y, marker='x')
    #plt.text(x + offset, y - 1.5, '1000 km challenge')

    #plt.scatter(1000, 92, marker='x', color='red')

    plt.title('Long Distance Capability of EV at 130 km/h')
    plt.xlabel('Distance travelled (km)')
    plt.ylabel('Average speed (km/h)')
    plt.legend()
    #plt.yscale('log')
    #print(plt.xaxis.get_majortcklocs())
    plt.minorticks_on()
    plt.grid(True, which='major', linewidth=0.3)
    plt.grid(True, which='minor', linewidth=0.1)
    fig1 = plt.gcf()
    fig1.text(0.05, 0.02, 'Julien Gautier (' + str(datetime.date.today()) + ')', fontsize=10, color='gray', ha='left', va='bottom', alpha=0.5)
    plt.show()
    plt.draw()
    fig1.savefig('graph.png')
