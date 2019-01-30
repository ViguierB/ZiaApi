/*
 * --------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <benjamin.viguier@epitech.eu> wrote this file. As long as you retain this 
 * notice you can do whatever you want with this stuff. If we meet some day,
 * and you think this stuff is worth it, you can buy me a beer in
 * return Benjamin Viguier
 * --------------------------------------------------------------------------
 */

#include "Event/Event.hpp"

namespace evt
{
	Event::Event(std::function<void ()> const &onEmpty_):
		onEmpty(onEmpty_) {}

	#if !defined(NDEBUG)
	void	Event::dump() const
	{
		std::cout << "{" << std::endl;
		for (auto &evtType : _hdls) {
			std::cout << "\t[\"" << evtType.first.c_str() << "\"] = " <<
				evtType.second.size() << ',' << std::endl;
		}
		std::cout << "}" << std::endl;
	}
	#endif

	bool	Event::isEmpty(void) const
	{
		return (_hdls.empty());
	}

	void	Event::_addToList(std::string const &tpl, Event::EvtHdlBlockDestr *blk)
	{
		_hdls[tpl].push_back(blk);
		auto it = _hdls[tpl].end();
		it--;
		blk->destructor = [this, &tpl, it, blk] {
			_hdls[tpl].erase(it);
			if (_hdls[tpl].empty())
				_hdls.erase(tpl);
			if (onEmpty != nullptr && _hdls.empty())
				onEmpty();
			delete blk;
		};
	}

	Event::~Event()
	{
		for (auto &evtType : _hdls) {
			for (auto ptr : evtType.second) {
				delete ptr;
			}
			evtType.second.clear();
		}
	}
}