#include <iostream>
#include "Zany.hpp"

int testsLoader() {
	zany::Loader	l;

	std::string		libPath;

	if constexpr (zany::isUnix) {
		libPath = "build-Linux/libSimpleModule.so";
	} else {
		libPath = "SimpleModule.dll";
	}

	auto &module = l.load(libPath);

	module.init();

	return (0);
}