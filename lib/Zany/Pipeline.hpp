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

	class Set {
	public:
		struct ID {
			std::uint16_t	PRSignValue;
			std::uint64_t	uniqueId;
		};

		Set() = default;

		template<Priority P = Priority::LOW, Rights R = Rights::READ_ONLY>
		inline ID	addTask(ThreadPool::Handler const &fct);
		inline void	execute();
		inline void	removeTask(ID id);
		inline void	linkTheadPool(ThreadPool &pool) { _pool = &pool; }

		inline auto	&getMutex() { return _mtx; }
	private:
		std::uint64_t	_genId()
			{ static std::uint64_t nextid = 0; return ++nextid; }

		template<Priority P, Rights R>
		struct PRSign {
			static constexpr std::uint16_t	value = ((std::uint16_t) P << 8) | (std::uint16_t) R;
		};
		std::map<
			std::uint16_t,
			std::unordered_map<
				std::uint64_t,
				ThreadPool::Handler
			>>		_handlers;
		std::mutex	_mtx;
		ThreadPool	*_pool;
	};

	class Instance {
	public:

	private:
		Instance() {}
		friend Pipeline;
	};

	inline void	linkTheadPool(ThreadPool &pool) { _pool = &pool; }

	template<Hooks H>
	inline Set	&getHookSet();

	static inline Pipeline	&instance();
private:
	Pipeline(): _pool(nullptr) {}

	ThreadPool	*_pool;
};

}

#include "Pipeline.ipp"