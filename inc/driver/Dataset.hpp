#pragma once

#include <string>
#include <fstream>
#include <cstdlib>
#include "inc/AdjacencyList/ALGraph.hpp"
#include "inc/AdjacencyMatrix/AMGraph.hpp"

class Dataset {
	struct VertexPointers {
		const Vertex<int>* al;
		const Vertex<int>* am;
	};

	double density;
	std::size_t num_vertices, start_idx;
	VertexPointers* vertices;
	ALGraph<int, int> algraph;
	AMGraph<int, int> amgraph;

public:
	Dataset(const std::string& path);
	~Dataset();

	std::size_t graphNumVertices() const;
	double graphDensity() const;

	const ALGraph<int, int>& getALGraph() const;
	const AMGraph<int, int>& getAMGraph() const;
	const Vertex<int>* getStartVertexAL() const;
	const Vertex<int>* getStartVertexAM() const;
};