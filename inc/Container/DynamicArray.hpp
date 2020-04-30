#pragma once

#include <memory>
#include <cstdlib>
#include <stdexcept>
#include "inc/Container/LinkedList.hpp"

template <typename T>
class DynamicArray {
	typedef std::shared_ptr<T> Tptr;

	Tptr array;
	std::size_t used;
	std::size_t allocated;
	std::size_t realloc_size = 5;

public:
	T* begin() const;
	T* end() const;

	DynamicArray(std::size_t preallocate = 0);
	DynamicArray(Tptr array, std::size_t used, std::size_t allocated);

	std::size_t size() const;
	std::size_t allocatedSize() const;
	void setSize(std::size_t new_size);

	const T& operator[](std::size_t idx) const;
	T& operator[](std::size_t idx);
	T* operator+(std::size_t idx);
	std::size_t find(T obj) const;

	T popLast();
	void add(T obj);

	void allocate(std::size_t count);
	void allocateMore(std::size_t count);
	void setReallocationSize(std::size_t size);

	template <typename To>
	DynamicArray<To> constCast() const {
		static_assert(sizeof(T) == sizeof(To), "can't cast: different type sizes");
		return DynamicArray<To>(std::const_pointer_cast<To>(this->array), this->used, this->allocated);
	}
};
