#!/usr/bin/env sh

BuildDir="build-$(uname)"

if [ "$(uname)" = "Linux" ]; then
	ExecutableName="zany-tests-bin"
	ExecutablePath="$BuildDir/$ExecutableName"
else
	ExecutableName="zany-tests-bin.exe"
	ExecutablePath="$BuildDir/Debug/$ExecutableName"
fi

if [ ! -d $BuildDir ]; then
	mkdir $BuildDir
fi

(
	cd $BuildDir
	cmake ..
	cmake --build . $@
) && echo "OK" || (echo "KO" && exit 1);

( $ExecutablePath ) &&
	echo "OK"
	exit 0
||
	echo "KO"
	exit 1
