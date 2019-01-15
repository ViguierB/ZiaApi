/*
** EPITECH PROJECT, 2018
** api
** File description:
** Loader.hpp
*/

#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <tuple>
#include "Platform.hpp"

namespace zany {

class Loader {
public:
	using ID = std::uint64_t;

	class AbstractModule {
	public:
		AbstractModule(ID uniqueId): _unique(uniqueId) {}

		auto	getUniqueId() const { return _unique; }
	private:
		ID	_unique;
	};

	inline AbstractModule	&load(std::string const &name);
	inline void				unload(AbstractModule const &module);
private:
	using Hdl = 
#	if defined(ZANY_ISUNIX)
		void *;
#	else
		HINSTANCE;
#	endif
	static inline const ID	_generateId()
		{ static ID nextId = 0; return ++nextId; }

	std::unordered_map <
		ID,
		std::tuple <
			void *,
			std::unique_ptr<AbstractModule>
		>
	>	_modules;
};



}