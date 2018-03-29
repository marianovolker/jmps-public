#!/bin/bash

if test $# -lt 1
then
	echo "ERROR: in argument received. Is needed the building arguments to build. Example: $0 -t"
	exit 1
fi

path="/home/hstools/workspace/source"
echo "cd $path"
cd $path
test $? -ne 0 && echo "Invalid working local copy at [$path]." && exit 2

echo "[$1]"

if test "$1" == "-a"
then
	echo "./build -5"
	./build -5

	echo "./build -3"
	./build -3

	echo "./build -w"
	./build -w

	echo "./build -t"
	./build -t
	
elif test "$1" == "-c" -a "$2" == "-a"
then
	echo "./build -c -5"
	./build -c -5

	echo "./build -c -3"
	./build -c -3

	echo "./build -c -w"
	./build -c -w

	echo "./build -c"
	./build -c

else
	BUILD_OPTIONS=$*
	echo "./build $BUILD_OPTIONS"
	exec ./build $BUILD_OPTIONS
fi

