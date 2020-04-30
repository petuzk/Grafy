#pragma once

#include <cstdlib>     // std::size_t
#include <stdexcept>   // std::invalid_argument
#include "inc/Algorithm/util.hpp"
#include "inc/Container/DynamicArray.hpp"
#include "inc/AdjacencyMatrix/AMEdge.hpp"
#include "inc/IndexableVertex/IVGraph.hpp"

template <typename VIT, typename EIT>
class AMGraph: public IVGraph<VIT, EIT> {
protected:
	std::size_t edges_count;
	DynamicArray<Edge<VIT, EIT>*> triangle_matrix;

	static std::size_t getMatrixIndex(std::size_t v_idx, std::size_t w_idx);
	DynamicArray<std::size_t> getMatrixIndices(std::size_t v_idx) const;

public:
	AMGraph();
	~AMGraph();

	const DynamicArray<const Edge<VIT, EIT>*> edges() const override;
	const DynamicArray<const Edge<VIT, EIT>*> incidentEdges(const Vertex<VIT>* v) const override;
	const Edge<VIT, EIT>* edgeBetween(const Vertex<VIT>* v, const Vertex<VIT>* w) const override;

	Vertex<VIT>* insertVertex(VIT value) override;
	Edge<VIT, EIT>* insertEdge(const Vertex<VIT>* v, const Vertex<VIT>* w, EIT value) override;
	void removeVertex(const Vertex<VIT>* v) override;
	void removeEdge(const Edge<VIT, EIT>* e) override;

	void preallocateForVertices(std::size_t count) override;
	void preallocateForEdges(std::size_t count) override;
};