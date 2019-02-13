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
	inline Orchestrator(InterfaceContext &ctx);
	Orchestrator(Orchestrator const &other) = delete;
	Orchestrator(Orchestrator &&other) = default;
	Orchestrator &operator=(Orchestrator const &other) = delete;

	/*
	** Change un module de façon asynchrone
	*/
	inline void	loadModule(
		std::string const &filename,
		std::function<void(Loader::AbstractModule &)> const &callback,
		std::function<void(std::exception)> const &error = nullptr);
	
	/*
	** Déchange un module de façon asynchrone
	*/
	inline void unloadModule(
		Loader::AbstractModule const &module,
		std::function<void()> const &callback,
		std::function<void(std::exception)> const &error = nullptr);

	/*
	** Start une pipeline d'execution apres la reception d'une connexion
	*/
	inline void startPipeline(zany::Socket sockFd);

	/*
	** Permet de lancer l'execution des safe handler
	*/
	inline void waitForSafeHandlersFinished();

	/*
	** Ajouter un safe handler a la queue, un safe handler est une
	**  fonction qui doit etre executé dans un context sécurisé
	**  par exemple le chargement d'un module
	*/
	template<typename T> inline void	addSafeHandler(T &&function);

	/*
	** Fonction de routine, elle est executé a chaque tour de boucle (doit etre override)
	*/
	virtual	void	routine() = 0;

	/*
	** Lie une threadpool au gestionnaire de pipeline
	*/
	inline void	linkThreadPool(ThreadPool &pool) { _pline.linkThreadPool(pool); }
	inline auto	&getThreadPool() { return _pline.getThreadPool(); }
	inline auto	&getThreadPool() const { return _pline.getThreadPool(); }
	inline auto	&getLoader() const { return _loader; }
	inline auto &getPipeline() { return _pline; }
	inline auto const &getPipeline() const { return _pline; }
protected:
	InterfaceContext	&_ctx;
	Pipeline			_pline;
	Loader				_loader;
private:
	inline void	_routine();

	Loader::AbstractModule				*_coreModule = nullptr;
	std::deque<std::function<void()>>	_safeHdls;
	std::mutex							_safeMtx;
	std::atomic<bool>					_safeIsComputing = false;
	std::vector<zany::Socket>			_waitSafeConnections;
};

}

#include "Orchestrator.ipp"