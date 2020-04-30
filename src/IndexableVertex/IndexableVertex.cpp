#include "inc/IndexableVertex/IndexableVertex.hpp"

template <typename VIT, typename EIT>
IndexableVertex<VIT, EIT>::IndexableVertex(std::size_t index, VIT item): index(index), Vertex<VIT>(item) { }

template <typename VIT, typename EIT>
std::size_t IndexableVertex<VIT, EIT>::getIndex() const {
	return index;
}

template class IndexableVertex<int, int>;