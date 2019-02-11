/*
** EPITECH PROJECT, 2018
** api
** File description:
** Context.hpp
*/

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace zany {

class InterfaceContext {
public:
	using Handler = std::function<void()>;

	InterfaceContext() = default;
	virtual ~InterfaceContext() = default;

	/*
	** Ajoute une tache
	*/
	virtual void	addTask(Handler const &handler) = 0;

	/*
	** attend tant que la queue d'execution n'est pas vide
	*/
	virtual void	waitUntilEmpty() = 0;

	/*
	** Demarre (blockant)
	*/
	virtual void	run() = 0;

	/*
	** Stop le context
	*/
	virtual void	stop() = 0;
};

/*
** Une implementation de context
*/
class Context : public InterfaceContext {
public:
	Context() = default;
	Context(Context const &other) = delete;
	Context(Context &&other) = default;
	Context &operator=(Context const &other) = delete;


	virtual inline void	addTask(Handler const &handler) final;
	virtual inline void	waitUntilEmpty() final;
	virtual inline void	run() final;
	virtual inline void	stop() final;
private:
	std::mutex				_hdlsLock;
	std::queue<Handler>		_hdls;
	std::condition_variable	_notLongerEmptyNotifier;
	std::condition_variable	_emptyNotifier;
	bool					_continue = false;
};

}

#include "Context.ipp"