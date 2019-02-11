//
// Created by seb on 30/01/19.
//

#pragma once

#include "Header.hpp"

namespace zany {
	class Response {
	public:
		Response(std::string const &protocole,
			 std::string const &protocoleVersion,
			 unsigned int const &status):
			protocole(protocole),
			protocoleVersion(protocoleVersion),
			status(status)
		{}

		Response(std::string const &protocole,
			std::string const &protocoleVersion,
			unsigned int const &status,
			std::unordered_map<std::string, Header> const &headers):
		protocole(protocole),
		protocoleVersion(protocoleVersion),
		status(status),
		headers(headers)
		{}

		std::string					protocole;
		std::string					protocoleVersion;
		unsigned int 					status;
		std::unordered_map<std::string, Header>		headers;
	};
}
