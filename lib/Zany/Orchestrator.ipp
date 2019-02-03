/*
** EPITECH PROJECT, 2018
** api
** File description:
** Orchestrator.ipp
*/

#pragma once

#include "Orchestrator.hpp"

namespace zany {

void	Orchestrator::loadModule(std::string const &filename, std::function<void(Loader::AbstractModule &)> const &callback, std::function<void(std::exception)> const &errorCallback) {
	_addSafeHandler([this, filename, callback, errorCallback] {
		try {
			auto &module = _loader.load(filename);
			_ctx.addTask(std::bind(callback, module));
		} catch (std::exception &e) {
			if (errorCallback != nullptr)
				_ctx.addTask(std::bind(errorCallback, e));
		}
	});
}

void	Orchestrator::unloadModule(Loader::AbstractModule const &module, std::function<void()> const &callback) {
	_addSafeHandler([this, &module, callback] {
		_loader.unload(module);

		_ctx.addTask(callback);
	});
}

void	Orchestrator::startPipeline(zany::Socket sockFd) {
	if (_safeIsComputing == false) {
		//_pline.startPipeline(sockFd);
	} else {
		_waitSafeConnections.push_back(sockFd);
	}
}

void	Orchestrator::waitForSafeHandlersComputed() {
	_ctx.addTask([this] {
		_safeIsComputing = true;
		_pline.getThreadPool().waitForEmpty();
		{
			std::lock_guard<decltype(_safeMtx)>	_guard(_safeMtx);
			for (auto &hdl: _safeHdls) {
				hdl();
			}
			_safeHdls.clear();
		}
		_safeIsComputing = false;
		std::this_thread::yield();
		for (auto fd: _waitSafeConnections) {
			startPipeline(fd);
		}
		_waitSafeConnections.clear();
	});
}

template<typename T> void	Orchestrator::_addSafeHandler(T &&handler) {
	std::lock_guard<decltype(_safeMtx)>	_guard(_safeMtx);

	_safeHdls.push_back(handler);
}


}