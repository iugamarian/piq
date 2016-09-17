[chutsu@alarmpi scripts]$ sudo python
[sudo] password for chutsu: 
Python 3.4.3 (default, Mar 26 2015, 06:33:18) 
[GCC 4.9.2 20141224 (prerelease)] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import RPi.GPIO as GPIO
>>> GPIO.setmode(GPIO.BOARD)
>>> GPIO.setup(24, GPIO.OUT)                                                                                                                                                                                                                                                                                                   
__main__:1: RuntimeWarning: This channel is already in use, continuing anyway.  Use GPIO.setwarnings(False) to disable warnings.
>>> GPIO.setup(17, GPIO.OUT)                                                                                                                                                                                                                                                                                                   
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
ValueError: The channel sent is invalid on a Raspberry Pi
>>> GPIO.setup(16, GPIO.OUT)                                                                                                                                                                                                                                                                                                   
>>> GPIO.PWM(16, 50)        
<RPi.GPIO.PWM object at 0xb6a7f170>
>>> p = GPIO.PWM(16, 50)                                                                                                                                                                                                                                                                                                       
>>> p.start(10)
>>> p.start(4)                                                                                                                                                                                                                                                                                                                 
>>> p.ChangeDutyCycle(4)                                                                                                                                                                                                                                                                                                   
>>> p.ChangeDutyCycle(10)
>>> p.ChangeDutyCycle(4)
>>> p.ChangeDutyCycle(5)
>>> p.ChangeDutyCycle(6)
>>> p.ChangeDutyCycle(4)
>>> p.ChangeDutyCycle(4.5)
>>> p.ChangeDutyCycle(3)
>>> p.ChangeDutyCycle(5)
>>> p.ChangeDutyCycle(6)
>>> p.ChangeDutyCycle(3)
>>> p.ChangeDutyCycle(10)
>>> p.ChangeDutyCycle(4)
>>> p.ChangeDutyCycle(4.5)
>>> p.ChangeDutyCycle(4.1)
>>> p.ChangeDutyCycle(4.2)
>>> p.ChangeDutyCycle(4.3)
>>> p.stop
<built-in method stop of RPi.GPIO.PWM object at 0xb6a7f218>
>>> p.stop()
>>> 
