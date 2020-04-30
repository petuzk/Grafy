#include "inc/Base/Edge.hpp"


template <typename VIT, typename EIT>
Edge<VIT, EIT>::Edge(Vertex<VIT>* v, Vertex<VIT>* w, EIT value): v(v), w(w), Cell<EIT>(value) { }

template <typename VIT, typename EIT>
void Edge<VIT, EIT>::setV(Vertex<VIT>* x) {
	v = x;
}

template <typename VIT, typename EIT>
void Edge<VIT, EIT>::setW(Vertex<VIT>* x) {
	w = x;
}

template <typename VIT, typename EIT>
void Edge<VIT, EIT>::setOpposite(const Vertex<VIT>* opp_to, Vertex<VIT>* x) {
	if (opp_to == v)
		w = x;
	else if (opp_to == w)
		v = x;
}

template <typename VIT, typename EIT>
const Vertex<VIT>* Edge<VIT, EIT>::getV() const {
	return v;
}

template <typename VIT, typename EIT>
const Vertex<VIT>* Edge<VIT, EIT>::getW() const {
	return w;
}

template <typename VIT, typename EIT>
const Vertex<VIT>* Edge<VIT, EIT>::getOpposite(const Vertex<VIT>* opp_to) const {
	if (opp_to == v)
		return w;
	else if (opp_to == w)
		return v;
	else
		return nullptr;
}

template <typename VIT, typename EIT>
Vertex<VIT>* Edge<VIT, EIT>::getV() {
	return const_cast<Vertex<VIT>*>(static_cast<const Edge<VIT, EIT>*>(this)->getV());
}

template <typename VIT, typename EIT>
Vertex<VIT>* Edge<VIT, EIT>::getW() {
	return const_cast<Vertex<VIT>*>(static_cast<const Edge<VIT, EIT>*>(this)->getW());
}

template <typename VIT, typename EIT>
Vertex<VIT>* Edge<VIT, EIT>::getOpposite(const Vertex<VIT>* opp_to) {
	return const_cast<Vertex<VIT>*>(static_cast<const Edge<VIT, EIT>*>(this)->getOpposite(opp_to));
}

template <typename VIT, typename EIT>
std::ostream& operator<<(std::ostream& stream, const Edge<VIT, EIT>* e) {
	if (!e)
		return stream << "nullptr";

	return stream << "Edge(v = " << e->getV()
	              <<    ", w = " << e->getW()
	              << ", item = " << e->getItem() << ')';
}

template class Edge<int, int>;
template std::ostream& operator<<(std::ostream& stream, const Edge<int, int>* v);