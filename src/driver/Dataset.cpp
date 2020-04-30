#include "inc/driver/Dataset.hpp"

Dataset::Dataset(const std::string& path) {
	int cost;
	std::size_t i, num_edges;
	std::size_t v_idx, w_idx;
	std::ifstream file(path);

	file >> num_edges >> num_vertices >> start_idx;
	density = 2.0 * num_edges / num_vertices / (num_vertices - 1);
	vertices = new VertexPointers[num_vertices];

	algraph.preallocateForEdges(num_edges);
	amgraph.preallocateForEdges(num_edges);
	algraph.preallocateForVertices(num_vertices);
	amgraph.preallocateForVertices(num_vertices);

	for (i = 0; i < num_vertices; i++) {
		vertices[i] = {
			algraph.insertVertex(static_cast<int>(i)),
			amgraph.insertVertex(static_cast<int>(i))
		};
	}

	for (i = 0; i < num_edges; i++) {
		file >> v_idx >> w_idx >> cost;
		algraph.insertEdge(vertices[v_idx].al, vertices[w_idx].al, cost);
		amgraph.insertEdge(vertices[v_idx].am, vertices[w_idx].am, cost);
	}

	file.close();
}

Dataset::~Dataset() {
	delete[] vertices;
}

std::size_t Dataset::graphNumVertices() const {
	return num_vertices;
}

double Dataset::graphDensity() const {
	return density;
}

const ALGraph<int, int>& Dataset::getALGraph() const {
	return algraph;
}

const AMGraph<int, int>& Dataset::getAMGraph() const {
	return amgraph;
}

const Vertex<int>* Dataset::getStartVertexAL() const {
	return vertices[start_idx].al;
}

const Vertex<int>* Dataset::getStartVertexAM() const {
	return vertices[start_idx].am;
}