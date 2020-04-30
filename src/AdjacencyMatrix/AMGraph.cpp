#include "inc/AdjacencyMatrix/AMGraph.hpp"

template <typename VIT, typename EIT>
std::size_t AMGraph<VIT, EIT>::getMatrixIndex(std::size_t v_idx, std::size_t w_idx) {
	if (v_idx == w_idx)
		throw std::invalid_argument("loops are unsupported");

	if (v_idx > w_idx)
		return triangular_number(v_idx - 1) + w_idx;
	else
		return triangular_number(w_idx - 1) + v_idx;
}

template <typename VIT, typename EIT>
DynamicArray<std::size_t> AMGraph<VIT, EIT>::getMatrixIndices(std::size_t v_idx) const {
	std::size_t num_vertices = IVGraph<VIT, EIT>::numVertices();
	DynamicArray<std::size_t> arr(num_vertices - 1);

	if (v_idx != 0) {
		// Dodaj krawędzie z wiersza
		std::size_t row_start = triangular_number(v_idx - 1);
		for (std::size_t m_idx = row_start; m_idx < row_start + v_idx; m_idx++)
			arr.add(m_idx);
	}

	if (v_idx != num_vertices - 1) {
		// Dodaj krawędzie z kolumny
		for (std::size_t row = v_idx + 1; row < num_vertices; row++)
			arr.add(triangular_number(row - 1) + v_idx);
	}

	return arr;
}

template <typename VIT, typename EIT>
AMGraph<VIT, EIT>::AMGraph(): edges_count(0), IVGraph<VIT, EIT>() { /* do nothing */ }

template <typename VIT, typename EIT>
AMGraph<VIT, EIT>::~AMGraph() {
	for (const Edge<VIT, EIT>* e: triangle_matrix) {
		if (e)
			delete e;
	}
}

template <typename VIT, typename EIT>
const DynamicArray<const Edge<VIT, EIT>*> AMGraph<VIT, EIT>::edges() const {
	DynamicArray<const Edge<VIT, EIT>*> arr(edges_count);

	for (const Edge<VIT, EIT>* e: triangle_matrix) {
		if (e)
			arr.add(e);
	}

	return arr;
}

template <typename VIT, typename EIT>
const DynamicArray<const Edge<VIT, EIT>*> AMGraph<VIT, EIT>::incidentEdges(const Vertex<VIT>* v) const {
	DynamicArray<const Edge<VIT, EIT>*> arr;

	for (std::size_t m_idx: getMatrixIndices(IVGraph<VIT, EIT>::getVertexIndex(v))) {
		const Edge<VIT, EIT>* e = triangle_matrix[m_idx];
		if (e)
			arr.add(e);
	}

	return arr;
}

template <typename VIT, typename EIT>
const Edge<VIT, EIT>* AMGraph<VIT, EIT>::edgeBetween(const Vertex<VIT>* v, const Vertex<VIT>* w) const {
	return triangle_matrix[getMatrixIndex(IVGraph<VIT, EIT>::getVertexIndex(v), IVGraph<VIT, EIT>::getVertexIndex(w))];
}

template <typename VIT, typename EIT>
Vertex<VIT>* AMGraph<VIT, EIT>::insertVertex(VIT value) {
	bool reused;
	Vertex<VIT>* v = IVGraph<VIT, EIT>::insertIVertex(value, reused);

	if (!reused) {
		std::size_t v_idx = IVGraph<VIT, EIT>::getVertexIndex(v);
		if (v_idx > 0) {
			// Zaalokuj nowe miejsce w macierzy
			std::size_t i, old_size = triangular_number(v_idx - 1), new_size = old_size + v_idx;
			triangle_matrix.setSize(new_size);
			for (i = old_size; i < new_size; i++)
				triangle_matrix[i] = nullptr;
		}
	}

	return v;
}

template <typename VIT, typename EIT>
Edge<VIT, EIT>* AMGraph<VIT, EIT>::insertEdge(const Vertex<VIT>* v, const Vertex<VIT>* w, EIT value) {
	Vertex<VIT>* ncv = const_cast<Vertex<VIT>*>(v);
	Vertex<VIT>* ncw = const_cast<Vertex<VIT>*>(w);

	Edge<VIT, EIT>*& edge = triangle_matrix[getMatrixIndex(IVGraph<VIT, EIT>::getVertexIndex(v), IVGraph<VIT, EIT>::getVertexIndex(w))];

	if (!edge) {
		edges_count++;
		return edge = new AMEdge<VIT, EIT>(ncv, ncw, value);
	}

	throw std::logic_error("edge already exists");
}

template <typename VIT, typename EIT>
void AMGraph<VIT, EIT>::removeVertex(const Vertex<VIT>* v) {
	std::size_t v_idx;
	IVGraph<VIT, EIT>::removeIVertex(v, v_idx);

	for (std::size_t m_idx: getMatrixIndices(v_idx)) {
		delete triangle_matrix[m_idx];
		triangle_matrix[m_idx] = nullptr;
	}
}

template <typename VIT, typename EIT>
void AMGraph<VIT, EIT>::removeEdge(const Edge<VIT, EIT>* e) {
	std::size_t m_idx = getMatrixIndex(IVGraph<VIT, EIT>::getVertexIndex(e->getV()), IVGraph<VIT, EIT>::getVertexIndex(e->getW()));

	edges_count--;
	delete triangle_matrix[m_idx];
	triangle_matrix[m_idx] = nullptr;
}

template <typename VIT, typename EIT>
void AMGraph<VIT, EIT>::preallocateForVertices(std::size_t count) {
	IVGraph<VIT, EIT>::preallocateForVertices(count);
	triangle_matrix.allocate(triangular_number(count - 1));
}

template <typename VIT, typename EIT>
void AMGraph<VIT, EIT>::preallocateForEdges(std::size_t count) { /* do nothing */ }

template class AMGraph<int, int>;