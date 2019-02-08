//
// Created by seb on 04/02/19.
//

#pragma once
#include <regex>
#include <iostream>

namespace zany {
	class Header {
	public:
		Header(): data(){};
		Header(std::string const &data): data(data){}

		inline bool		isNumber();
		inline double		getNumber();

		inline bool		isDate();

		inline std::string	const &getData();

	private:
		std::string	data;
	};
}


#include "Header.ipp"