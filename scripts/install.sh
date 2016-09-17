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


# MAIN
install_libbcm2835
