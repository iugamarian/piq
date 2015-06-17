# Pulse Width Modulation

Frequency does not matter

Servos / ESCs expect 1ms to 2ms pulse for 0% 100% position / throttle.

Duty Cycle = ((Pulse Width time) / (Time Period)) * 100



# Hardware Solution

Better to use a hardware solution using a 555 time for 1 servo signal. But we
can use a 568 quad timer to drive 4 servo signals. +1 Digital Potentiometer to
vary the resistors that control the duty signal of a 555 timer.
