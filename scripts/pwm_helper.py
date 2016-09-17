#!/usr/bin/env python


def duty_cycle(freq):
    period = 1 / float(freq)
    duty_cycle_low = (0.001 / period) * 100
    duty_cycle_high = (0.002 / period) * 100

    print duty_cycle_low, duty_cycle_high


if __name__ == "__main__":
    duty_cycle(50)
    duty_cycle(400)
