# piq
`piq` is a Raspberry Pi Quadrotor project.


## Hardware
The project uses the following hardware:

- Raspberry Pi Model B (On board flight computer)
- MPU6050 (Inertia Measurement Unit)
- PCA9685 (Electronic Speed Controller)


## Software Dependencies

- [`libbcm2835`][1]



## Install

    git clone https://github.com/chutsu/piq.git
    cd piq
    make


## Licence
GPLv3 Copyright (C) <2015> Chris Choi

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <http://www.gnu.org/licenses/>.


[1]: http://www.airspayce.com/mikem/bcm2835/
