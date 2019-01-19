/*
** EPITECH PROJECT, 2018
** api
** File description:
** Pipeline.ipp
*/

#pragma once

#include "Pipeline.hpp"

namespace zany {

template<>
struct Pipeline::Set::_FunctionTypeSelector<Pipeline::Rights::READ_ONLY> {
	using type = std::function<void(Pipeline::Instance const &)>;
};
template<>
struct Pipeline::Set::_FunctionTypeSelector<Pipeline::Rights::READ_WRITE> {
	using type = std::function<void(Pipeline::Instance &)>;
};

Pipeline	&Pipeline::master() {
	static Pipeline	me;

	return me;
}

template<Pipeline::Priority P, Pipeline::Rights R>
inline Pipeline::Set::ID	Pipeline::Set::addTask(typename _FunctionTypeSelector<R>::type const &fct) {
	std::lock_guard<decltype(_mtx)>	_guard(_mtx);
	auto newId = _genId();

	_handlers[PRSign<P, R>::value][newId] = (std::unique_ptr<void>(reinterpret_cast<void*>(new decltype(fct)(fct))));
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

void	Pipeline::Set::execute(ThreadPool &pool, Pipeline::Instance &pipeline) {
	for (auto &group: _handlers) {
		auto r = static_cast<Rights>(group.first | 0xFF);
		if (r == Rights::READ_ONLY) {
			std::condition_variable			isDone;
			std::mutex						mtx;
			std::unique_lock<decltype(mtx)>	lock;
			std::atomic<std::uint32_t>		runCounter(group.second.size());

			for (auto &hdl: group.second) {
				pool.runTask([&] {
					reinterpret_cast<typename _FunctionTypeSelector<Rights::READ_ONLY>::type*>(hdl.second.get())->operator()(pipeline);
					--runCounter;
					if (runCounter <= 0)
						isDone.notify_all();
				});
			}
			isDone.wait(lock);
		} else {
			for (auto &hdl: group.second) {
				reinterpret_cast<typename _FunctionTypeSelector<Rights::READ_WRITE>::type*>(hdl.second.get())->operator()(pipeline);
			}
		}
	}
}

}