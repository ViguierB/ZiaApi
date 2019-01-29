/*
** EPITECH PROJECT, 2018
** api
** File description:
** AbstractStream.hpp
*/

#pragma once

#include <iostream>
#include "Platform.hpp"

#if defined(ZANY_ISUNIX)
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
#else
# include <io.h>
#endif

namespace zany {

class NetworkIO {
public:
	inline NetworkIO(int socketFd): _socketFd(socketFd) {}

	inline auto	write(char const *buffer, std::size_t len)
		{ return ::send(_socketFd, buffer, len, _sendFlags); }
	inline auto	read(char *buffer, std::size_t len)
		{ return ::recv(_socketFd, buffer, len, _recvFlags); }

	inline void setFd(int fd) { _socketFd = fd; }
	inline auto getFd() { return _socketFd; }

	inline void setSendFlags(int flags) { _sendFlags = flags; }
	inline auto getSendFlags() { return _sendFlags; }

	inline void setRecvFlags(int flags) { _recvFlags = flags; }
	inline auto getRecvFlags() { return _recvFlags; }
private:
	int _sendFlags = 0;
	int _recvFlags = 0;
	int	_socketFd;
};

template<typename IO = NetworkIO>
class NetworkStreamBuf: public std::streambuf, private IO {
public:
	NetworkStreamBuf(int fd): IO(fd), _buffer{} {};
protected:
	virtual int_type underflow() override {
		
	}
	virtual int_type uflow() override;
	virtual int_type pbackfail(int_type ch) override;
	virtual std::streamsize showmanyc() override;
private:
	std::array<int_type, 1024>	_buffer;
};

class Stream {
};

}