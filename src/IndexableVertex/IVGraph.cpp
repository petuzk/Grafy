#include "inc/IndexableVertex/IVGraph.hpp"

template <typename VIT, typename EIT>
std::size_t IVGraph<VIT, EIT>::numVertices() const {
	return graph_vertices.size() - unused_indices.size();
}

template <typename VIT, typename EIT>
IndexableVertex<VIT, EIT>* IVGraph<VIT, EIT>::castVertexGetIndex(const Vertex<VIT>* v, std::size_t& idx_ref	) const {
	IndexableVertex<VIT, EIT>* amv = const_cast<IndexableVertex<VIT, EIT>*>(dynamic_cast<const IndexableVertex<VIT, EIT>*>(v));
	idx_ref = amv->getIndex();

	if (graph_vertices.size() > idx_ref && graph_vertices[idx_ref] == v)
		return amv;

	throw std::invalid_argument("given vertex does not belong to the graph");
}

template <typename VIT, typename EIT>
IndexableVertex<VIT, EIT>* IVGraph<VIT, EIT>::castVertex(const Vertex<VIT>* v) const {
	std::size_t idx;
	return castVertexGetIndex(v, idx);
}

template <typename VIT, typename EIT>
std::size_t IVGraph<VIT, EIT>::getVertexIndex(const Vertex<VIT>* v) const {
	std::size_t idx;
	castVertexGetIndex(v, idx);
	return idx;
}

template <typename VIT, typename EIT>
Vertex<VIT>* IVGraph<VIT, EIT>::insertIVertex(VIT value, bool& reused) {
	std::size_t v_idx;
	reused = unused_indices.size() > 0;

	if (reused) {
		// Wykorzystaj zwolnione miejsce ponownie
		v_idx = unused_indices.popLast();
	} else {
		v_idx = graph_vertices.size();
	}

	IndexableVertex<VIT, EIT>* v = new IndexableVertex<VIT, EIT>(v_idx, value);
	graph_vertices.add(v);
	return v;
}

template <typename VIT, typename EIT>
void IVGraph<VIT, EIT>::removeIVertex(const Vertex<VIT>* v, std::size_t& v_idx) {
	v_idx = getVertexIndex(v);
	unused_indices.add(v_idx);

	delete graph_vertices[v_idx];
	graph_vertices[v_idx] = nullptr;
}

template <typename VIT, typename EIT>
IVGraph<VIT, EIT>::IVGraph() { /* do nothing */ }

template <typename VIT, typename EIT>
IVGraph<VIT, EIT>::~IVGraph() {
	for (const Vertex<VIT>* v: graph_vertices) {
		if (v)
			delete v;
	}
}

template <typename VIT, typename EIT>
const DynamicArray<const Vertex<VIT>*> IVGraph<VIT, EIT>::vertices() const {
	std::size_t num_vertices = numVertices();
	DynamicArray<const Vertex<VIT>*> arr(num_vertices);

	for (const Vertex<VIT>* v: graph_vertices) {
		if (v)
			arr.add(v);
	}

	return arr;
}

template <typename VIT, typename EIT>
void IVGraph<VIT, EIT>::preallocateForVertices(std::size_t count) {
	graph_vertices.allocate(count);
}

template class IVGraph<int, int>;