#pragma once

#include <cstdlib>     // std::size_t
#include <stdexcept>   // std::invalid_argument
#include "inc/Container/LinkedList.hpp"
#include "inc/Container/DynamicArray.hpp"
#include "inc/AdjacencyList/ALEdge.hpp"
#include "inc/IndexableVertex/IVGraph.hpp"

template <typename VIT, typename EIT>
class ALGraph: public IVGraph<VIT, EIT> {
protected:
	std::size_t edges_count;
	DynamicArray<LinkedList<Edge<VIT, EIT>*>> connections;

	unsigned char unvisitedEdgeFlagValue() const;
	unsigned char visitedEdgeFlagValue(unsigned char unvisited) const;

public:
	ALGraph();
	~ALGraph();

	const DynamicArray<const Edge<VIT, EIT>*>  edges() const override;
	const DynamicArray<const Edge<VIT, EIT>*>  incidentEdges(const Vertex<VIT>* v) const override;
	                   const Edge<VIT, EIT>*   edgeBetween(const Vertex<VIT>* v, const Vertex<VIT>* w) const override;

	Vertex<VIT>* insertVertex(VIT value) override;
	Edge<VIT, EIT>* insertEdge(const Vertex<VIT>* v, const Vertex<VIT>* w, EIT value) override;
	void removeVertex(const Vertex<VIT>* v) override;
	void removeEdge(const Edge<VIT, EIT>* e) override;

	void preallocateForVertices(std::size_t count) override;
	void preallocateForEdges(std::size_t count) override;
};