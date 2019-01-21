/*
** EPITECH PROJECT, 2018
** api
** File description:
** Pipeline.hpp
*/

#pragma once

#include <exception>
#include "ThreadPool.hpp"

namespace zany {

class Pipeline {
public:
	enum class Hooks {
		AFTER_ACCEPT,
		BEFORE_HTTP_HEADER_INTERPRETATION,
		AFTER_HTTP_HEADER_INTERPRETATION
		/* ... */
	};

	enum class Priority: std::uint8_t {
		HIGH,
		MEDIUM,
		LOW,
	};
	enum class Rights: std::uint8_t {
		READ_ONLY,
		READ_WRITE
	};

	class Instance;

	class Set {
	public:
		struct ID {
			std::uint16_t	PRSignValue;
			std::uint64_t	uniqueId;
		};

		Set() = default;

		template<Rights R>
		struct _FunctionTypeSelector {};

		template<Priority P = Priority::LOW, Rights R = Rights::READ_WRITE>
		inline ID	addTask(typename _FunctionTypeSelector<R>::type const &fct);
		inline void	execute(ThreadPool &pool, Instance &pipeline);
		inline void	removeTask(ID id);

		/* maybe useless ... */
		inline auto	&getMutex() { return _mtx; }
	private:
		std::uint64_t	_genId()
			{ static std::uint64_t nextid = 0; return ++nextid; }

		std::map<
			std::uint16_t,
			std::unordered_map<
				std::uint64_t,
				std::shared_ptr<void>
			>>		_handlers;
		std::mutex	_mtx;
	};

	class Instance {
	public:
		Instance() {}
	private:

	};

	inline void	linkTheadPool(ThreadPool &pool) { _pool = &pool; }

	template<Hooks H>
	inline Set	&getHookSet();
	template<Hooks H>
	inline void	executeHook(Instance &instance)
		{ getHookSet<H>().execute(*_pool, instance); }

	static inline Pipeline	&master();
private:
	Pipeline(): _pool(nullptr) {}

	ThreadPool	*_pool;
};

}

#include "Pipeline.ipp"