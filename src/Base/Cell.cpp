#include "inc/Base/Cell.hpp"

template <typename T>
Cell<T>::Cell(T obj) {
	setItem(obj);
}

template <typename T>
const T& Cell<T>::getItem() const {
	return item;
}

template <typename T>
void Cell<T>::setItem(T obj) {
	item = obj;
}

#include "inc/AbstractGraph.hpp"

template class Cell<Edge<int, int>*>;
template class Cell<const Edge<int, int>*>;

template class Cell<int>;