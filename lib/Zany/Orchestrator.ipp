/*
** EPITECH PROJECT, 2018
** api
** File description:
** Orchestrator.ipp
*/

#pragma once

#include "Orchestrator.hpp"

namespace zany {

template<typename T, typename std::enable_if<std::is_base_of<Orchestrator, T>::value, int>::type>
Orchestrator::Orchestrator(InterfaceContext &ctx, void (T::*member)())
:  _ctx(ctx), __routine(_CastUtility<T>(member).ptr) {
	ctx.addTask(std::bind(&Orchestrator::_routine, this));
}

void	Orchestrator::loadModule(std::string const &filename, std::function<void(Loader::AbstractModule &)> const &callback, std::function<void(zany::Loader::Exception)> const &errorCallback) {
	const auto lbd = [this, filename, callback, errorCallback] {
		try {
			auto &module = _loader.load(filename);

			module.linkOrchestrator(*this);
			if (module.isACoreModule()) {
				if (_coreModule != nullptr) {
					throw std::runtime_error("Cannot load a core module when another is already loaded !");
				}
				_coreModule = &module;
			}
			module.init();
			if (_ctx.running()) {
				_ctx.addTask(std::bind(callback, std::ref(module)));
			} else {
				callback(module);
			}
		} catch (zany::Loader::Exception &e) {
			if (!_ctx.running()) {
				errorCallback(e);
				return;
			}
			if (errorCallback != nullptr)
				_ctx.addTask(std::bind(errorCallback, e));
		}
	};
	
	if (_ctx.running()) {
		addSafeHandler(lbd);
		waitForSafeHandlersFinished();
	} else {
		lbd();
	}
}

void	Orchestrator::unloadModule(Loader::AbstractModule const &module, std::function<void()> const &callback, std::function<void(zany::Loader::Exception)> const &errorCallback) {
	const auto lbd = [this, &module, callback, errorCallback] {
		try {
			if (_coreModule == &module) {
				_coreModule = nullptr;
			}
			
			_loader.unload(module);
			if (_ctx.running()) {
				_ctx.addTask(callback);
			} else {
				callback();
			}
		} catch (zany::Loader::Exception &e) {
			if (!_ctx.running()) {
				errorCallback(e);
				return;
			}
			if (errorCallback != nullptr)
				_ctx.addTask(std::bind(errorCallback, e));
		}
	};

	if (_ctx.running()) {
		addSafeHandler(lbd);
		waitForSafeHandlersFinished();
	} else {
		lbd();
	}
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
	if (this->__routine) {
		(this->*_CastUtility<Orchestrator>(__routine).routine)();
	} else {
		_ctx.waitWhileEmpty();
	}
	_ctx.addTask(std::bind(&Orchestrator::_routine, this));
}


}