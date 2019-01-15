/*
** EPITECH PROJECT, 2018
** api
** File description:
** LoaderUnix.ipp
*/

#pragma once

#include <dlfcn.h>
#include "Loader.hpp"

namespace zany {

Loader::AbstractModule &Loader::load(std::string const &filename) {
	AbstractModule	&(*entrypoint)(ID);
	Hdl				ptr = 
#	if defined(ZANY_ISUNIX)
		::dlopen(filename.c_str(), RTLD_LAZY);
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

	auto &module = entrypoint(reinterpret_cast<ID>(ptr));
	_modules.insert(
		std::make_pair(
			module.getUniqueId(),
			std::make_tuple(
				ptr,
				std::make_unique<AbstractModule>(&module))));
	return module;
}

void	Loader::unload(AbstractModule const &module) {
	auto id = module.getUniqueId();

	auto it = _modules.find(id);
	if (it == _modules.end()) {
		//TODO: handle this error correctly
		throw std::exception();
	}
	
	void *hdl = std::get<0>(it->second);
	_modules.erase(it);

	if (
#	if defined(ZANY_ISUNIX)
		::dlclose
#	else
		FreeLibrary
#	endif
	(hdl)) {
		//TODO: handle this error correctly
		throw std::exception();
	}
}

}