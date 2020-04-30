#include "inc/Base/Vertex.hpp"

template <typename VIT>
Vertex<VIT>::Vertex(VIT item): Cell<VIT>(item) { }

template <typename VIT>
std::ostream& operator<<(std::ostream& stream, const Vertex<VIT>* v) {
	if (!v)
		return stream << "nullptr";

	return stream << "Vertex(item = " << v->getItem() << ')';
}

template class Vertex<int>;
template std::ostream& operator<<(std::ostream& stream, const Vertex<int>* v);