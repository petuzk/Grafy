#pragma once

#include <cstdlib>
#include <stdexcept>
#include "inc/Base/Cell.hpp"

template <typename T>
class LinkedList {
public:
	class Iterator;

	class Element: public Cell<T> {
		friend class LinkedList<T>;
		friend class LinkedList<T>::Iterator;

	protected:
		Element* next;
		Element* prev;
		LinkedList<T>* ll;

		Element(T item, LinkedList<T>* ll);

	public:
		LinkedList<T>* getLL();
	};

	class Iterator {
		friend class LinkedList<T>;

	protected:
		Element* e;

		Iterator(Element* first = nullptr);

	public:
		Element* operator*();
		Element* derefUnsafe();
		void operator++();
		bool operator!=(Iterator& other);
	};

protected:
	Element* first;
	Element* last;
	std::size_t llsize;

public:
	LinkedList();
	~LinkedList();

	bool operator==(const LinkedList& other) const;

	Iterator begin() const;
	Iterator end() const;

	std::size_t size() const;

	Element* insertAfter(T item, Element* after = nullptr);
	Element* insertBefore(T item, Element* before = nullptr);
	void remove(Element* e);
	void clear();
};