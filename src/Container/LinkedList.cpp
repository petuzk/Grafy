#include "inc/Container/LinkedList.hpp"

template <typename T>
LinkedList<T>::Element::Element(T item, LinkedList<T>* ll): ll(ll), Cell<T>(item) { }

template <typename T>
LinkedList<T>* LinkedList<T>::Element::getLL() {
	return ll;
}

template <typename T>
LinkedList<T>::Iterator::Iterator(LinkedList<T>::Element* first): e(first) { }

template <typename T>
typename LinkedList<T>::Element* LinkedList<T>::Iterator::operator*() {
	if (!e)
		throw std::out_of_range("can't dereference iterator at the end");
	return e;
}

template <typename T>
typename LinkedList<T>::Element* LinkedList<T>::Iterator::derefUnsafe() {
	return e;
}

template <typename T>
void LinkedList<T>::Iterator::operator++() {
	if (!e)
		throw std::out_of_range("can't increment iterator at the end");
	e = e->next;
}

template <typename T>
bool LinkedList<T>::Iterator::operator!=(Iterator& other) {
	return derefUnsafe() != other.derefUnsafe();
}

template <typename T>
LinkedList<T>::LinkedList(): first(nullptr), last(nullptr), llsize(0) { }

template <typename T>
LinkedList<T>::~LinkedList() {
	clear();
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList& other) const {
	return false;
}

template <typename T>
typename LinkedList<T>::Iterator LinkedList<T>::begin() const {
	return Iterator(first);
}

template <typename T>
typename LinkedList<T>::Iterator LinkedList<T>::end() const {
	return Iterator();
}

template <typename T>
std::size_t LinkedList<T>::size() const {
	return llsize;
}

template <typename T>
typename LinkedList<T>::Element* LinkedList<T>::insertAfter(T item, Element* after) {
	if (!after)
		after = last;

	Element* new_elem = new Element(item, this);
	new_elem->prev = after;

	if (after) {
		if ((new_elem->next = after->next))
			new_elem->next->prev = new_elem;
		else
			last = new_elem;
		after->next = new_elem;
	} else {
		new_elem->next = nullptr;
		first = last = new_elem;
	}

	llsize++;
	return new_elem;
}

template <typename T>
typename LinkedList<T>::Element* LinkedList<T>::insertBefore(T item, Element* before) {
	if (!before)
		before = first;

	Element* new_elem = new Element(item, this);
	new_elem->next = before;

	if (before) {
		if ((new_elem->prev = before->prev))
			new_elem->prev->next = new_elem;
		else
			first = new_elem;
		before->prev = new_elem;
	} else {
		new_elem->prev = nullptr;
		first = last = new_elem;
	}

	llsize++;
	return new_elem;
}

template <typename T>
void LinkedList<T>::remove(Element* e) {
	if (e->prev)
		e->prev->next = e->next;
	else
		first = e->next;

	if (e->next)
		e->next->prev = e->prev;
	else
		last = e->prev;

	llsize--;
	delete e;
}

template <typename T>
void LinkedList<T>::clear() {
	Element* curr = first;
	Element* next;

	while (curr) {
		next = curr->next;
		delete curr;
		curr = next;
	}
}

#include "inc/Base/Edge.hpp"

template class LinkedList<Edge<int, int>*>;
template class LinkedList<const Edge<int, int>*>;