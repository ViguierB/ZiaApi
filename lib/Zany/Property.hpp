/*
** EPITECH PROJECT, 2018
** api
** File description:
** MutableObject.hpp
*/

#pragma once

namespace zany {

class PropertyBase {
public:
	PropertyBase(void *ptr): _ptr(ptr) {}
	PropertyBase(PropertyBase const &) = default;
	PropertyBase(PropertyBase &&) = default;
	virtual ~PropertyBase() = default;

	template<typename T> const T	&get() const { return *reinterpret_cast<T*>(_ptr); }
	template<typename T> T			&get() { return *reinterpret_cast<T*>(_ptr); }
protected:
	void	*_ptr;
};

template<typename BT>
class Property : private PropertyBase {
public:
	using type = typename std::remove_const<BT>::type;

	~Property() { delete reinterpret_cast<type*>(_ptr); }

	template<typename ...Args>
	static PropertyBase make(Args &&...args) {
		return Property(new type(std::forward<Args>(args)...));
	}
private:
	Property(type *ptr): PropertyBase(ptr) {}
};

} // zany
