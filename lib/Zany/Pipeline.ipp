/*
** EPITECH PROJECT, 2018
** api
** File description:
** Pipeline.ipp
*/

#pragma once

#include "Pipeline.hpp"

namespace zany {

Pipeline	&Pipeline::instance() {
	static Pipeline	me;

	return me;
}

template<Pipeline::Priority P, Pipeline::Rights R>
inline Pipeline::Set::ID	Pipeline::Set::addTask(std::function<void()> const &fct) {
	std::lock_guard<decltype(_mtx)>	_guard(_mtx);
	auto newId = _genId();

	_handlers[PRSign<P, R>::value][newId](fct);
	return { PRSign<P, R>::value, newId };
}

namespace __hidden {
template<Pipeline::Hooks H>
struct _SetRegister {
	static inline Pipeline::Set	value;
};
}

template<Pipeline::Hooks H>
Pipeline::Set	&Pipeline::getHookSet() {
	return __hidden::_SetRegister<H>::value;
}

void	Pipeline::Set::execute() {
	for (auto &group: _handlers) {
		auto r = static_cast<Rights>(group.first | 0xFF);
		if (r == Rights::READ_ONLY) {
			std::condition_variable			isDone;
			std::mutex						mtx;
			std::unique_lock<decltype(mtx)>	lock;
			std::atomic<std::uint32_t>		runCounter(group.second.size());

			for (auto &hdl: group.second) {
				_pool->runTask([&] {
					hdl.second();
					--runCounter;
					if (runCounter <= 0)
						isDone.notify_all();
				});
			}
			isDone.wait(lock);
		} else {
			for (auto &hdl: group.second) {
				hdl.second();
			}
		}
	}
}

}