#!/bin/sh
export TARGET=10.230.5.243
export SYSROOT=${HOME}/Genivi/GDP-SDK-RPi3/sysroots/cortexa7hf-neon-vfpv4-poky-linux-gnueabi
tar -C  gdp-hmi -cvf - gdp-hmi | ssh root@${TARGET} 'tar -C /usr/bin -xvf -'
tar -C  ${SYSROOT}/usr/lib/qt5/qml -cvf - com | ssh root@${TARGET} 'tar -C /usr/lib/qt5/qml -xvf -'
