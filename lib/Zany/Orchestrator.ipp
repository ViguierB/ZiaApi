/*
** EPITECH PROJECT, 2018
** api
** File description:
** Orchestrator.ipp
*/

#pragma once

#include "Orchestrator.hpp"

namespace zany {

Orchestrator::Orchestrator(InterfaceContext &ctx): _ctx(ctx) {
	ctx.addTask(std::bind(&Orchestrator::_routine, this));
}

void	Orchestrator::loadModule(std::string const &filename, std::function<void(Loader::AbstractModule &)> const &callback, std::function<void(std::exception)> const &errorCallback) {
	addSafeHandler([this, filename, callback, errorCallback] {
		try {
			auto &module = _loader.load(filename);
			if (module.isCoreModule()) {
				if (_coreModule != nullptr) {
					throw std::runtime_error("Cannot load a core module when another is already loaded !");
				}
				_coreModule = &module;
			}
			_ctx.addTask(std::bind(callback, std::ref(module)));
		} catch (std::exception &e) {
			if (errorCallback != nullptr)
				_ctx.addTask(std::bind(errorCallback, e));
		}
	});
	waitForSafeHandlersFinished();
}

void	Orchestrator::unloadModule(Loader::AbstractModule const &module, std::function<void()> const &callback, std::function<void(std::exception)> const &errorCallback) {
	addSafeHandler([this, &module, callback, errorCallback] {
		try {
			if (_coreModule == &module) {
				_coreModule = nullptr;
			}
			
			_loader.unload(module);
			_ctx.addTask(callback);
		} catch (std::exception &e) {
			if (errorCallback != nullptr)
				_ctx.addTask(std::bind(errorCallback, e));
		}
	});
	waitForSafeHandlersFinished();
}

void	Orchestrator::startPipeline(zany::Socket sockFd) {
	if (_safeIsComputing == false) {
		if (_coreModule == nullptr) {
			throw std::runtime_error("Core module never set !!");
		}
		_coreModule->coreBeginPipeline(sockFd);
	} else {
		_waitSafeConnections.push_back(sockFd);
	}
}

void	Orchestrator::waitForSafeHandlersFinished() {
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

template<typename T> void	Orchestrator::addSafeHandler(T &&handler) {
	std::lock_guard<decltype(_safeMtx)>	_guard(_safeMtx);

	_safeHdls.push_back(handler);
}

void	Orchestrator::_routine() {
	this->routine();
	_ctx.addTask(std::bind(&Orchestrator::_routine, this));
}


}