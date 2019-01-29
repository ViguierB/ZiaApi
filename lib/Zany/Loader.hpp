/*
** EPITECH PROJECT, 2018
** api
** File description:
** Loader.hpp
*/

#pragma once

#if defined(ZANY_ISUNIX)
# include <dlfcn.h>
#else
# include <windows.h>
#endif

#include <unordered_map>
#include <string>
#include <memory>
#include <tuple>
#include <array>
#include "Pipeline.hpp"
#include "Platform.hpp"

namespace zany {

class Loader {
public:
	using ID = std::uintptr_t;

	class AbstractModule {
	private:
		ID			_unique;
	public:
		AbstractModule(): _unique(reinterpret_cast<ID>(this)) {}
		virtual inline ~AbstractModule() = default;

		virtual void	init() = 0;
		auto			getUniqueId() const { return _unique; }
		void			linkMasterPipeline(Pipeline &p) { master = &p; }
	protected:
		class Collector {
		public:
			inline ~Collector();
			Pipeline::Set::ID	operator<<(Pipeline::Set::ID id)
				{ _handlerIDs[_handlerLen++] = id; return id; }
		private:
			std::array<Pipeline::Set::ID, Pipeline::Hooks::count()>
						_handlerIDs;
			std::size_t	_handlerLen = 0;
		};
		Collector	garbage;
		Pipeline	*master = nullptr;
	};

	inline AbstractModule	&load(std::string const &name);
	inline void				unload(AbstractModule const &module);
private:
	class ModuleWrapper {
	public:
		using Handler =
#		if defined(ZANY_ISUNIX)
			void *;
#		else
			HINSTANCE;
#		endif

		ModuleWrapper(Handler h, AbstractModule *am)
			: _h(h), _m(am) {}
		~ModuleWrapper() {
			delete _m;
#			if defined(ZANY_ISUNIX)
				::dlclose(_h);
#			else
				FreeLibrary(_h);
#			endif
		}
	private:
		Handler			_h;
		AbstractModule	*_m;
	};

	std::unordered_map<ID, std::unique_ptr<ModuleWrapper>>	_modules;
};

}

#include "Loader.ipp"