#!/usr/bin/env python
import csv
import matplotlib.pyplot as plt

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
            print(f'Column names are {", ".join(row)}')
            line_count += 1
        else:
            print(row)
            numeric = [float(t) for t in row[1:]]
            plt.plot(numeric)
            #print(f'\t{row[0]} works in the {row[1]} department, and was born in {row[2]}.')
            line_count += 1
    print(f'Processed {line_count} lines.')
    plt.show()
