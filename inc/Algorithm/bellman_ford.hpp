#pragma once

#include "inc/Base/Vertex.hpp"
#include "inc/IndexableVertex/IVGraph.hpp"

/*
 * Struktury pomocnicze dla algorytmu
 */

template <typename VIT, typename EIT>
struct BFVertex {
	const Vertex<VIT>* tree_vertex;
	BFVertex<VIT, EIT>* predecessor;
	EIT cost;
};

template <typename VIT, typename EIT>
struct BFEdge {
	BFVertex<VIT, EIT>* v;
	BFVertex<VIT, EIT>* w;
	EIT cost;
};



template <typename VIT, typename EIT>
void bellman_ford(const IVGraph<VIT, EIT>& graph, IVGraph<VIT, EIT>& tree, const Vertex<VIT>* start, EIT zero_cost, EIT max_cost);