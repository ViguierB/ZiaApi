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

	bool Header::isDate() {
		std::regex pattern { "/^(?:(Sun|Mon|Tue|Wed|Thu|Fri|Sat),\s+)?(0[1-9]|[1-2]?[0-9]|3[01])\s+(Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec)\s+(19[0-9]{2}|[2-9][0-9]{3})\s+(2[0-3]|[0-1][0-9]):([0-5][0-9])(?::(60|[0-5][0-9]))?\s+([-\+][0-9]{2}[0-5][0-9]|(?:UT|GMT|(?:E|C|M|P)(?:ST|DT)|[A-IK-Z]))(\s+|\(([^\(\)]+|\\\(|\\\))*\))*$/abc" };
	}
}