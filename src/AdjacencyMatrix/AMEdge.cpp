#include "inc/AdjacencyMatrix/AMEdge.hpp"

template <typename VIT, typename EIT>
AMEdge<VIT, EIT>::AMEdge(Vertex<VIT>* v, Vertex<VIT>* w, EIT value):
	Edge<VIT, EIT>(v, w, value) { /* do nothing */ }

template class AMEdge<int, int>;