#!/usr/bin/env python
import RPi.GPIO as GPIO
import time


if __name__ == "__main__":
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(23, GPIO.OUT, pull_up_down=GPIO.PUD_DOWN)
    GPIO.output(23, GPIO.LOW)

    while True:
        time.sleep(1)
