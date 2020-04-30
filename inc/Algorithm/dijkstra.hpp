#pragma once

#include "inc/Algorithm/heapsort.hpp"
#include "inc/IndexableVertex/IVGraph.hpp"

/*
 * Struktura pomocnicza dla algorytmu
 */

template <typename VIT, typename EIT>
struct VertexWithCost {
	const Vertex<VIT>* graph_vertex;
	const Vertex<VIT>* tree_vertex;
	VertexWithCost<VIT, EIT>* predecessor;
	EIT cost;

	bool operator < (const VertexWithCost& other) const {
		return cost < other.cost;
	}

	bool operator > (const VertexWithCost& other) const {
		return cost > other.cost;
	}
};

template <typename VIT, typename EIT>
void dijkstra(const IVGraph<VIT, EIT>& graph, IVGraph<VIT, EIT>& tree, const Vertex<VIT>* start, EIT zero_cost, EIT max_cost);