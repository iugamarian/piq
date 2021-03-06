#!/usr/bin/env python
import csv

import matplotlib.pyplot as plt


if __name__ == "__main__":
    data = []
    data_file = open("output.dat")
    reader = csv.reader(data_file, delimiter=",")

    gyro_xyz_data = []
    gyro_pitch = []
    gyro_roll = []

    accel_xyz_data = []
    accel_pitch = []
    accel_roll = []

    pitch = []
    roll = []

    for row in reader:
        gyro_xyz_data.append(row[:3])
        gyro_pitch.append(row[3])
        gyro_roll.append(row[4])

        accel_xyz_data.append(row[5:8])
        accel_pitch.append(row[8])
        accel_roll.append(row[9])

        pitch.append(row[10])
        roll.append(row[11])

    plt.plot(gyro_pitch, label="gyro pitch")
    plt.plot(accel_pitch, label="accel pitch")
    plt.plot(pitch, label="pitch")
    plt.xlim([0, len(gyro_pitch)])
    plt.ylim([-90, 90])
    plt.legend(loc='upper left')
    plt.show()

    plt.plot(gyro_roll, label="gyro roll")
    plt.plot(accel_roll, label="accel roll")
    plt.plot(roll, label="roll")
    plt.xlim([0, len(gyro_pitch)])
    plt.ylim([-90, 90])
    plt.legend(loc='upper left')
    plt.show()
