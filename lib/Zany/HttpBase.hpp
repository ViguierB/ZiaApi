//
// Created by seb on 30/01/19.
//

#pragma once

#include <string>
#include <unordered_map>
#include "HttpHeader.hpp"

namespace zany {

struct HttpBaseRequest {
	std::string									protocol;
	std::string									protocolVersion;
	std::unordered_map<std::string, HttpHeader>	headers;
};

struct HttpRequest: public HttpBaseRequest {
	enum class RequestMethods: std::uint8_t {
		GET = 42,
		HEAD,
		POST,
		PUT,
		_DELETE_, // DELETE is a keyword for MSVC... 
		TRACE,
		OPTIONS,
		CONNECT,
		PATCH,
	};

	RequestMethods	method;
};

struct HttpResponse: public HttpBaseRequest {
	unsigned int 	status;
};

}
