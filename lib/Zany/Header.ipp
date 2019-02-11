//
// Created by seb on 04/02/19.
//

#pragma once

#include "Header.hpp"
#include <cstdlib>

namespace zany {
	std::string	const& Header::getData() {
		return data;
	}

	bool Header::isNumber() {
		double isn = std::atof(data);

		if (isn != 0.0)
			return true;
		return data == "0.0";
	}

	double Header::getNumber() {
		return std::atof(data);
	}
}