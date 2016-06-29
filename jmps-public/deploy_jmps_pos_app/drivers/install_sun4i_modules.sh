#!/bin/sh

SUN4IPATCH_ROOT=.
rc=0

QDEVICES_CONNECTED=$(adb devices | grep -w device | wc -l)
if [ "$QDEVICES_CONNECTED" -ne 1 ]
then
	echo "More than one devices are connected to the system..."
	exit 1
fi

TABLET_VERSION=$(adb shell busybox uname -r | tr -d '\r')
if [ "$TABLET_VERSION" = "3.0.8+" ]
then
	SUN4IPATCH_ROOT="./Model_B07"
else
	SUN4IPATCH_ROOT="./Model_T01"
fi

echo "Using patch for [$SUN4IPATCH_ROOT] [$TABLET_VERSION] ..."

# Copying USB-Serial Converters drivers
adb shell mount -o rw,remount -t rootfs rootfs / ; rc=$(expr $? + $rc)
adb push $SUN4IPATCH_ROOT/ftdi_sio.ko /system/vendor/modules/; rc=$(expr $? + $rc)
adb push $SUN4IPATCH_ROOT/pl2303.ko /system/vendor/modules/; rc=$(expr $? + $rc)
adb remount; rc=$(expr $? + $rc)

adb reboot; rc=$(expr $? + $rc)

test $rc -eq 0 && echo "SUCCESS..." && exit 0
echo "FAILURE..." && exit 1


