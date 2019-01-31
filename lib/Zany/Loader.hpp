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
#include <variant>
#include <iterator>
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

		virtual void		init() = 0;
		virtual bool		isAParser() { return false; };
		virtual Entity		parse(std::string const &filename) { return false; }
		virtual auto		name() -> const std::string& = 0;
		auto				getUniqueId() const { return _unique; }
		void				linkMasterPipeline(Pipeline &p) { master = &p; }
		static inline bool	isValidParseResult(Entity const &variant);
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

	class Iterator;

	Loader() = default;
	Loader(Loader const &other) = delete;
	Loader(Loader &&other) = default;
	Loader &operator=(Loader const &other) = delete;

	inline AbstractModule	&load(std::string const &name);
	inline void				unload(AbstractModule const &module);
	inline void				unloadAll();
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

		friend Loader;
		friend Loader::Iterator;
	};

	std::unordered_map<ID, std::unique_ptr<ModuleWrapper>>	_modules;
public:
	class Iterator: public std::iterator<std::input_iterator_tag, struct epoll_event> {
	public:
		using Type = AbstractModule;

		Iterator() = default;
		Iterator(decltype(Loader::_modules)::iterator &&it): _current(it) {}
		Iterator(const Iterator& mit): _current(mit._current) {}
		Iterator& operator++()
			{ ++_current; return *this; }
		Iterator operator++(int)
			{ Iterator tmp(*this); operator++(); return tmp; }
		bool operator==(const Iterator &rhs) const
			{ return _current == rhs._current; }
		bool operator!=(const Iterator &rhs) const
			{ return _current != rhs._current; }
		Type &operator*() { return *_current->second->_m; }
		Type *operator->() { return _current->second->_m; }
	private:
		decltype(Loader::_modules)::iterator	_current;

		friend Loader;
	};

	inline Iterator			begin() { return _modules.begin(); }
	inline Iterator			end() { return _modules.end(); }
	inline Iterator::Type	&front() { return *(Iterator(_modules.begin())); }
	inline std::size_t		size() { return _modules.size(); }
	inline Iterator			erase(Iterator it) { return _modules.erase(it._current); } 
};

}

#include "Loader.ipp"