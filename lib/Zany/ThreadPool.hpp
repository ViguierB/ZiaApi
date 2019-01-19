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

	ThreadPool(std::uint32_t threadNbr);
	~ThreadPool();

	inline std::uint32_t	available();
	inline void				runTask(Handler const &hdl);
	inline void				waitForEmpty();

	// abort is a dangerous function that should only be used in the most extreme cases
	// /!\ Usage of the ThreadPool instance after will have an unspecified behaviour
	inline void				abort();
private:
	void	_entrypoint();

	std::uint32_t				_nbr;
	std::atomic<std::uint32_t>	_inUse;
	std::queue<Handler>			_handlers;
	std::vector<std::thread>	_threads;
	bool						_aborted = false;

	bool						_continue = true;
	std::condition_variable 	_readyNotifier;
	std::condition_variable		_emptyNotifier;
	bool						_isReady = false;
	std::mutex					_lockPeek;
};

}

#include "ThreadPool.ipp"