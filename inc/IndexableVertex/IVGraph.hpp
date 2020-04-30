#pragma once

#include <cstdlib>     // std::size_t
#include <stdexcept>   // std::invalid_argument
#include "inc/AbstractGraph.hpp"
#include "inc/Container/DynamicArray.hpp"
#include "inc/IndexableVertex/IndexableVertex.hpp"

template <typename VIT, typename EIT>
class IVGraph: public AbstractGraph<VIT, EIT> {
protected:
	DynamicArray<std::size_t> unused_indices;
	DynamicArray<Vertex<VIT>*> graph_vertices;

	std::size_t numVertices() const;

	IndexableVertex<VIT, EIT>* castVertexGetIndex(const Vertex<VIT>* v, std::size_t& idx_ref) const;
	IndexableVertex<VIT, EIT>* castVertex(const Vertex<VIT>* v) const;
	std::size_t getVertexIndex(const Vertex<VIT>* v) const;

	Vertex<VIT>* insertIVertex(VIT value, bool& reused);
	void removeIVertex(const Vertex<VIT>* v, std::size_t& v_idx);

public:
	IVGraph();
	~IVGraph();

	/*
	 * Gwarantowane jest, że kolejność wierzchołków w tablicy z vertices()
	 * będzie taka sama jak kolejność ich dodania dopóki żaden wierzchołek
	 * nie będzie usunięty.
	 */
	const DynamicArray<const Vertex<VIT>*> vertices() const override;
	void preallocateForVertices(std::size_t count) override;
};

