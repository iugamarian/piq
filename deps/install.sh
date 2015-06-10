#!/bin/sh
set -e


install_libbcm2835()
{
    BCM2835_VERSION=1.44
    BCM2835_BASE_URL=http://www.airspayce.com/mikem/bcm2835
    BCM2835_FILE=bcm2835-$BCM2835_VERSION.tar.gz
    BCM2835_DOWNLOAD_URL=$BCM2835_BASE_URL/$BCM2835_FILE

    # download libbcm2835
    wget -nc $BCM2835_DOWNLOAD_URL

    # expand and enter dir
    tar -xzvf bcm2835-$BCM2835_VERSION.tar.gz
    cd bcm2835-$BCM2835_VERSION

    # make and install
    ./configure
    make
    make install

    # exit and clean up
    cd ..
    rm -rf bcm2835-$BCM2835_VERSION
    rm $BCM2835_FILE
}

install_pi_blaster()
{
    # clone pi-blaster repo
    git clone https://github.com/sarfata/pi-blaster

    # cd into repo
    cd pi-blaster

    # build and install
    ./autogen.sh
    ./configure
    make
    make install

    # clean up
    cd ..
}

# install_libbcm2835
install_pi_blaster
