#!/bin/sh
set -e  # exit on first error

LIBBCM_VERSION="1.50"
LIBBCM="bcm2835-${LIBBCM_VERSION}"
LIBBCM_URL="http://www.airspayce.com/mikem/bcm2835/${LIBBCM}.tar.gz"


install_libbcm2835()
{
    # download and extract libbcm2835
	cd /tmp
	wget $LIBBCM_URL
	tar -xzvf /tmp/${LIBBCM}.tar.gz

    # configure, make and install
	cd $LIBBCM
	./configure
	make
	sudo make check
	sudo make install

	# clean up
	rm -rf /tmp/${LIBBCM}
	rm -f /tmp/${LIBBCM}.tar.gz
}

install_libftdi()
{
    sudo apt-get update
    sudo apt-get install \
        build-essential \
        libusb-1.0-0-dev \
        swig \
        cmake \
        python-dev \
        libconfuse-dev \
        libboost-all-dev

    wget http://www.intra2net.com/en/developer/libftdi/download/libftdi1-1.2.tar.bz2
    tar xvf libftdi1-1.2.tar.bz2
    cd libftdi1-1.2

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX="/usr/" ../
    make
    sudo make install
}


# MAIN
install_libbcm2835
# install_libftdi
