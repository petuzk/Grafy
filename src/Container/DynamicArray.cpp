#include "inc/Container/DynamicArray.hpp"

template <typename T>
DynamicArray<T>::DynamicArray(std::size_t preallocate):
	array(nullptr, [](T* p) { delete[] p; }), used(0), allocated(0)
{
	allocate(preallocate);
}

template <typename T>
DynamicArray<T>::DynamicArray(Tptr array, std::size_t used, std::size_t allocated):
	array(array/*, [](T* p) { delete[] p; }*/), used(used), allocated(allocated) { }

// template <typename T>
// DynamicArray<T>::DynamicArray(LinkedList<T>& ll): used(0), allocated(0) {
// 	setSize(ll.size());
// 	std::size_t i = 0;

// 	for (Cell<T>* elem: ll)
// 		(*this)[i++] = elem->getItem();
// }

template <typename T>
T* DynamicArray<T>::begin() const {
	return array.get();
}

template <typename T>
T* DynamicArray<T>::end() const {
	return array.get() + used;
}

template <typename T>
std::size_t DynamicArray<T>::size() const {
	return used;
}

template <typename T>
std::size_t DynamicArray<T>::allocatedSize() const {
	return allocated;
}

template <typename T>
void DynamicArray<T>::setSize(std::size_t new_size) {
	if (new_size > allocated)
		allocate(new_size);

	used = new_size;
}

template <typename T>
const T& DynamicArray<T>::operator[](std::size_t idx) const {
	if (idx >= used)
		throw std::out_of_range("index exceeds array bounds");

	return array.get()[idx];
}

template <typename T>
T& DynamicArray<T>::operator[](std::size_t idx) {
	return const_cast<T&>(static_cast<const DynamicArray<T>>(*this)[idx]);
}

template <typename T>
T* DynamicArray<T>::operator+(std::size_t idx) {
	return &(operator[](idx));
}

template <typename T>
std::size_t DynamicArray<T>::find(T obj) const {
	for (std::size_t i = 0; i < used; i++) {
		if (operator[](i) == obj)
			return i;
	}

	return used;
}

template <typename T>
T DynamicArray<T>::popLast() {
	if (!used)
		throw std::out_of_range("array is empty");

	return array.get()[--used];
}

template <typename T>
void DynamicArray<T>::add(T obj) {
	if (used == allocated)
		allocateMore(realloc_size);

	array.get()[used++] = obj;
}

template <typename T>
void DynamicArray<T>::allocate(std::size_t count) {
	if (count <= allocated)
		return;

	Tptr new_array = Tptr(new T[count], [](T* p) { delete[] p; });

	for (std::size_t i = 0; i < used; i++)
		new_array.get()[i] = array.get()[i];

	array = new_array;
	allocated = count;
}

template <typename T>
void DynamicArray<T>::allocateMore(std::size_t count) {
	allocate(allocated + count);
}

template <typename T>
void DynamicArray<T>::setReallocationSize(std::size_t size) {
	if (!size)
		throw std::out_of_range("reallocation size must be greater than zero");

	realloc_size = size;
}

#include "inc/AbstractGraph.hpp"

template class DynamicArray<Vertex<int>*>;
template class DynamicArray<Edge<int, int>*>;
template class DynamicArray<const Vertex<int>*>;
template class DynamicArray<const Edge<int, int>*>;

#include "inc/IndexableVertex/IndexableVertex.hpp"

template class DynamicArray<IndexableVertex<int, int>*>;
template class DynamicArray<const IndexableVertex<int, int>*>;

#include "inc/driver/DatasetGenerator.hpp"

template class DynamicArray<DatasetGenerator::FirstGroup>;
template class DynamicArray<DatasetGenerator::Edge>;

template class DynamicArray<LinkedList<Edge<int, int>*>>;

template class DynamicArray<std::size_t>;