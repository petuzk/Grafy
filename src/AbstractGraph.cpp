#include "inc/AbstractGraph.hpp"

template <typename VIT, typename EIT>
const DynamicArray<Vertex<VIT>*> AbstractGraph<VIT, EIT>::vertices() {
	return static_cast<const AbstractGraph<VIT, EIT>*>(this)->vertices().template constCast<Vertex<VIT>*>();
}

template <typename VIT, typename EIT>
const DynamicArray<Edge<VIT, EIT>*> AbstractGraph<VIT, EIT>::edges() {
	return static_cast<const AbstractGraph<VIT, EIT>*>(this)->edges().template constCast<Edge<VIT, EIT>*>();
}

template <typename VIT, typename EIT>
const DynamicArray<Edge<VIT, EIT>*> AbstractGraph<VIT, EIT>::incidentEdges(const Vertex<VIT>* v) {
	return static_cast<const AbstractGraph<VIT, EIT>*>(this)->incidentEdges(v).template constCast<Edge<VIT, EIT>*>();
}

template <typename VIT, typename EIT>
Edge<VIT, EIT>* AbstractGraph<VIT, EIT>::edgeBetween(const Vertex<VIT>* v, const Vertex<VIT>* w) {
	return const_cast<Edge<VIT, EIT>*>(static_cast<const AbstractGraph<VIT, EIT>*>(this)->edgeBetween(v, w));
}

// template <typename VIT, typename EIT>
// std::ostream& operator<<(std::ostream& stream, const AbstractGraph<VIT, EIT>& graph) {

// }

// template <typename VIT, typename EIT>
// std::istream& operator>>(std::istream& stream, AbstractGraph<VIT, EIT>& graph) {

// }

template class AbstractGraph<int, int>;
// template std::ostream& operator<<(std::ostream& stream, const AbstractGraph<int, int>& graph);
// template std::istream& operator>>(std::istream& stream, AbstractGraph<int, int>& graph);