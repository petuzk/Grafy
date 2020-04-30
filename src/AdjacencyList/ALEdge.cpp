#include "inc/AdjacencyList/ALEdge.hpp"

template <typename VIT, typename EIT>
ALEdge<VIT, EIT>::ALEdge(Vertex<VIT>* v, Vertex<VIT>* w, EIT value, unsigned char flag):
	flag(flag), Edge<VIT, EIT>(v, w, value) { /* do nothing */ }

template class ALEdge<int, int>;