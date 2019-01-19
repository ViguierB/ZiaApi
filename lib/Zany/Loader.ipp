/*
** EPITECH PROJECT, 2018
** api
** File description:
** LoaderUnix.ipp
*/

#pragma once

#include "Loader.hpp"

namespace zany {

Loader::AbstractModule &Loader::load(std::string const &filename) {
	AbstractModule			*(*entrypoint)();
	ModuleWrapper::Handler	ptr = 
#	if defined(ZANY_ISUNIX)
		::dlopen(filename.c_str(), RTLD_LAZY); // If you have a leak here, it's a libdl issue (https://bugzilla.redhat.com/show_bug.cgi?id=1624387)
#	else
		LoadLibrary(filename.c_str());
#	endif

	if (ptr == nullptr) {
		//TODO: Handle tis error correctly;
		throw std::exception();
	}

	entrypoint = reinterpret_cast<decltype(entrypoint)>(
#	if defined(ZANY_ISUNIX)
		::dlsym
#	else
		GetProcAddress
#	endif
			(ptr, "entrypoint")
	);
	if (entrypoint == nullptr) {
		//TODO: Handle tis error correctly;
		throw std::exception();
	}

	auto *module = entrypoint();

	_modules.insert(
		std::make_pair(
			module->getUniqueId(),
			std::make_unique<ModuleWrapper>(ptr, module)));
	return *module;
}

void	Loader::unload(AbstractModule const &module) {
	auto id = module.getUniqueId();

	auto it = _modules.find(id);
	if (it == _modules.end()) {
		//TODO: handle this error correctly
		throw std::exception();
	}

	_modules.erase(it);
}

}