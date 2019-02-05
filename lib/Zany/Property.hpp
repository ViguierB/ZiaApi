/*
** EPITECH PROJECT, 2018
** api
** File description:
** MutableObject.hpp
*/

#pragma once

#include <memory>

namespace zany {

class Property {
public:
	Property() = default;
	Property(Property const &) = default;
	Property(Property &&) = default;

	Property &operator=(Property const &) = default;

	~Property() = default;

	template<typename _T, typename ..._Args>
	static inline Property make(_Args &&...__args) {
		using TargetType = typename _Data<_T>::type;
		return Property(new _Data<_T>(TargetType(__args...)));
	}

	template<typename _T, typename ..._Args>
	inline _T &set(_Args &&...__args) {
		using TargetType = typename _Data<_T>::type;
		_bdata = decltype(_bdata)(new _Data<_T>(TargetType(__args...)));
		return get<_T>();
	}

	template<typename _T> const _T	&get() const { return static_cast<_Data<_T>*>(_bdata.get())->data; }
	template<typename _T> _T		&get() { return static_cast<_Data<_T>*>(_bdata.get())->data; }
private:
	struct _BaseData {
		virtual ~_BaseData() = default;
	};
	template<typename _T> struct _Data: public _BaseData {
		~_Data() = default;

		using type = typename std::remove_const<_T>::type;

		_Data(type &&d): data(d) {}

		type	data;
	};
	Property(_BaseData *bdata): _bdata(bdata) {}

	std::shared_ptr<_BaseData>	_bdata = nullptr;
};

} // zany
