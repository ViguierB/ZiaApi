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

void	Orchestrator::loadModule(std::string const &filename, std::function<void(Loader::AbstractModule &)> const &callback, std::function<void(zany::Loader::Exception)> const &errorCallback) {
	addSafeHandler([this, filename, callback, errorCallback] {
		try {
			auto &module = _loader.load(filename);

			module.linkOrchestrator(*this);
			if (module.isCoreModule()) {
				if (_coreModule != nullptr) {
					throw std::runtime_error("Cannot load a core module when another is already loaded !");
				}
				_coreModule = &module;
			}
			_ctx.addTask(std::bind(callback, std::ref(module)));
		} catch (zany::Loader::Exception &e) {
			if (errorCallback != nullptr)
				_ctx.addTask(std::bind(errorCallback, e));
		}
	});
	waitForSafeHandlersFinished();
}

void	Orchestrator::unloadModule(Loader::AbstractModule const &module, std::function<void()> const &callback, std::function<void(zany::Loader::Exception)> const &errorCallback) {
	addSafeHandler([this, &module, callback, errorCallback] {
		try {
			if (_coreModule == &module) {
				_coreModule = nullptr;
			}
			
			_loader.unload(module);
			_ctx.addTask(callback);
		} catch (zany::Loader::Exception &e) {
			if (errorCallback != nullptr)
				_ctx.addTask(std::bind(errorCallback, e));
		}
	});
	waitForSafeHandlersFinished();
}

void	Orchestrator::startPipeline(zany::Connection::SharedInstance connection) {
	auto pipeline = Pipeline::makePipelineInstance(connection);

	if (_safeIsComputing == false) {
		this->_pline.getThreadPool().runTask([this, pipeline] {
			onPipelineReady(*pipeline);
			
		});
	} else {
		_waitSafeConnections.push_back(pipeline);
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
		for (auto &pipeline: _waitSafeConnections) {
			this->_pline.getThreadPool().runTask([this, pipeline] {
				onPipelineReady(*pipeline);
			});
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