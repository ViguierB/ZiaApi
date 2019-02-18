//
// Created by seb on 30/01/19.
//

#pragma once

#include <string>
#include <unordered_map>
#include "HttpHeader.hpp"

namespace zany {

struct HttpBase {
	std::string									protocole;
	std::string									protocoleVersion;
	
	std::unordered_map<std::string, HttpHeader>	headers;
};

struct HttpRequest: public HttpBase {
};

struct HttpResponse: public HttpBase {
	unsigned int 	status;
};
	
}
