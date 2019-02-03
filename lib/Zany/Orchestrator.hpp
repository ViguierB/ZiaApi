/*
** EPITECH PROJECT, 2018
** api
** File description:
** Orchestrator.hpp
*/

#pragma once

#include <functional>
#include <deque>
#include <mutex>
#include <atomic>
#include "./Platform.hpp"
#include "./Pipeline.hpp"
#include "./Loader.hpp"
#include "./Context.hpp"

namespace zany {

class Orchestrator {
public:
	Orchestrator(InterfaceContext &ctx): _ctx(ctx) {}
	Orchestrator(Orchestrator const &other) = delete;
	Orchestrator(Orchestrator &&other) = default;
	Orchestrator &operator=(Orchestrator const &other) = delete;

	inline void	loadModule(
		std::string const &filename,
		std::function<void(Loader::AbstractModule &)> const &callback,
		std::function<void(std::exception)> const &error = nullptr);
	inline void unloadModule(
		Loader::AbstractModule const &module,
		std::function<void()> const &callback);
	inline void startPipeline(zany::Socket sockFd);
	inline void waitForSafeHandlersComputed();

	inline void	linkThreadPool(ThreadPool &pool) { _pline.linkThreadPool(pool); }
	inline auto	&getThreadPool() { return _pline.getThreadPool(); }
	inline auto	&getThreadPool() const { return _pline.getThreadPool(); }
	inline auto	&getLoader() const { return _loader; }
private:
	template<typename T> void	_addSafeHandler(T &&function);

	std::deque<std::function<void()>>	_safeHdls;
	std::mutex							_safeMtx;
	std::atomic<bool>					_safeIsComputing = false;
	std::vector<zany::Socket>			_waitSafeConnections;
	InterfaceContext					&_ctx;
	Pipeline							_pline;
	Loader								_loader;
};

}

#include "Orchestrator.ipp"