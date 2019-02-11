//
// Created by seb on 30/01/19.
//

#pragma once

#include <string>
#include <unordered_map>
#include "HttpHeader.hpp"

namespace zany {

struct HttpBaseRequest {
	std::string									protocole;
	std::string									protocoleVersion;
	
	std::unordered_map<std::string, HttpHeader>	headers;
};

struct HttpRequest: public HttpBaseRequest {
};

struct HttpResponse: public HttpBaseRequest {
	unsigned int 	status;
};
	
}
