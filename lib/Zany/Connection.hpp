/*
** EPITECH PROJECT, 2018
** api
** File description:
** Connection.hpp
*/

#pragma once

#include <memory>

namespace zany {

class Connection: public std::enable_shared_from_this<Connection> {
public:
	virtual ~Connection() = default;

	using SharedInstance = std::shared_ptr<Connection>;

	/** 
	 * Called when data is available
	 * 
	 */
	virtual void	onDataAvailable(std::size_t len) = 0;

	/** 
	 * Write data on the socket
	 * 
	 */
	virtual void	write(const char *, std::size_t len) = 0;
};

}