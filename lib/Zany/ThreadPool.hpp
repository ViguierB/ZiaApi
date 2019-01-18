/*
** EPITECH PROJECT, 2018
** api
** File description:
** ThreadPool.hpp
*/

#pragma once

#include <condition_variable>
#include <map>
#include <queue>
#include <list>
#include <functional>
#include <cstdint>
#include <atomic>
#include <mutex>
#include <thread>

namespace zany {

class ThreadPool {
public:
	using Handler = std::function<void()>;
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
		inline ID	addTask(std::function<void()> const &fct);
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
				std::function<void()>
			>>		_handlers;
		std::mutex	_mtx;
		ThreadPool	*_pool;
	};

	ThreadPool(std::uint32_t threadNbr);
	~ThreadPool();

	inline std::uint32_t	available();
	inline void				runTask(Handler const &hdl);
	inline void				waitForEmpty();
private:
	void	_entrypoint();

	std::uint32_t				_nbr;
	std::atomic<std::uint32_t>	_inUse;
	std::queue<Handler>			_handlers;
	std::vector<std::thread>	_threads;

	bool						_continue = true;
	std::condition_variable 	_readyNotifier;
	std::condition_variable		_emptyNotifier;
	bool						_isReady = false;
	std::mutex					_lockPeek;
};

ThreadPool::ThreadPool(std::uint32_t threadNbr):
		_nbr(threadNbr), _inUse(0) {
	for (decltype(_nbr) i = 0; i < _nbr; ++i) {
		_threads.emplace_back(std::bind(&ThreadPool::_entrypoint, this));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	_isReady = true;
	_readyNotifier.notify_all();
}

ThreadPool::~ThreadPool() {
	_continue = false;
	_readyNotifier.notify_all();
	for (auto &t: _threads) {
		t.join();
	}
}

void	ThreadPool::_entrypoint() {
	std::mutex	mtx;
	{
		std::unique_lock<decltype(mtx)>	ulock(mtx);
		while (!_isReady) _readyNotifier.wait(ulock);
	}
	std::unique_lock<decltype(mtx)>	ulock(mtx);
	while (_continue) {
		Handler	hdl;
		while ([&] {
			std::lock_guard<decltype(_lockPeek)>	_guard(_lockPeek);
			return (_handlers.size() == 0 && _continue);
		}()) _readyNotifier.wait(ulock);
		if (!_continue) break;
		{
			std::lock_guard<decltype(_lockPeek)>	_guard(_lockPeek);
			if (_handlers.size() != 0) {
				hdl = _handlers.front();
				_handlers.pop();
			} else {
				continue;
			}
		}
		++_inUse;
		hdl();
		--_inUse;
		{
			std::lock_guard<decltype(_lockPeek)>	_guard(_lockPeek);

			if (_handlers.size() == 0 && available() == _nbr) {
				_emptyNotifier.notify_all();
			}
		}
	}
}

void	ThreadPool::runTask(Handler const &hdl) {
	bool	notify = false;
	{
		std::lock_guard<decltype(_lockPeek)>	_guard(_lockPeek);

		if (_handlers.size() == 0) notify = true;
		_handlers.push(hdl);
	}
	if (notify)
		_readyNotifier.notify_one();
}

std::uint32_t	ThreadPool::available() {
	return _nbr - _inUse.operator unsigned int();
}

void	ThreadPool::waitForEmpty() {
	std::mutex						mtx;
	std::unique_lock<decltype(mtx)>	ulock(mtx);
	bool 							needToWait;

retry:
	{
		std::lock_guard<decltype(_lockPeek)>	_guard(_lockPeek);

		needToWait = (_handlers.size() != 0 || available() != _nbr);
	}
	if (needToWait) {
		_emptyNotifier.wait(ulock);
		goto retry;
	}
}

template<ThreadPool::Priority P, ThreadPool::Rights R>
inline ThreadPool::Set::ID	ThreadPool::Set::addTask(std::function<void()> const &fct) {
	std::lock_guard<decltype(_mtx)>	_guard(_mtx);
	auto newId = _genId();

	_handlers[PRSign<P, R>::value][newId](fct);
	return { PRSign<P, R>::value, newId };
}

void	ThreadPool::Set::execute() {
	
}

}
