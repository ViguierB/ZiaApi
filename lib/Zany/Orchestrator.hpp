/*
** EPITECH PROJECT, 2018
** api
** File description:
** Orchestrator.hpp
*/

#pragma once

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

	inline void	loadModule(std::string const &filename);
	inline void unloadModule(Loader::ID id);

	inline void	linkThreadPool(ThreadPool &pool) { _pline.linkThreadPool(pool); }
	inline auto	&getThreadPool() { return _pline.getThreadPool(); }
	inline auto	&getThreadPool() const { return _pline.getThreadPool(); }
	inline auto	&getLoader() const { return _loader; }
private:
	InterfaceContext	&_ctx;
	Pipeline			_pline;
	Loader				_loader;
};

}