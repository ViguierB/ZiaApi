/*
 * --------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <benjamin.viguier@epitech.eu> wrote this file. As long as you retain this 
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in
 * return Benjamin Viguier
 * --------------------------------------------------------------------------
 */

#include "Event/HdlCollector.hpp"
#include <iostream>
namespace evt
{
	HdlCollector::~HdlCollector()
	{
		flush();
	}

	Event::EvtHdlDestr
	HdlCollector::add(Event::EvtHdlDestr const &hdl)
	{
		auto it = _unregisterEventsHdls.insert(_unregisterEventsHdls.end(), hdl);
		return ([this, it] () {
			auto fct = *it;
			this->_unregisterEventsHdls.erase(it);
			fct();
		});
	}

	HdlCollector
	&HdlCollector::operator<<(Event::EvtHdlDestr const &hdl)
	{
		_unregisterEventsHdls.push_back(hdl);
		return (*this);
	}

	void	HdlCollector::flush(void)
	{
		for (auto &destructor : _unregisterEventsHdls) {
			(destructor)();
		}
		_unregisterEventsHdls.clear();
	}
}