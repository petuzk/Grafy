#include "inc/Algorithm/bellman_ford.hpp"

template <typename VIT, typename EIT>
void bellman_ford(const IVGraph<VIT, EIT>& graph, IVGraph<VIT, EIT>& tree, const Vertex<VIT>* start, EIT zero_cost, EIT max_cost) {
	// Pobierz tablicę wierzchołków
	const auto graph_vertices = graph.vertices();
	std::size_t num_vertices = graph_vertices.size();

	// Zaalokuj pamięć dla nowych krawędzi i wierzchołków
	tree.preallocateForVertices(num_vertices);
	tree.preallocateForEdges(num_vertices - 1);

	// Utwórz tablicę wierzchołków specjalnych
	BFVertex<VIT, EIT>* vertices = new BFVertex<VIT, EIT>[num_vertices];

	// Dodaj wszystkie wierzchołki do drzewa i zainicjalizuj tablicę
	for (std::size_t i = 0; i < num_vertices; i++) {
		const Vertex<VIT>* gv = graph_vertices[i];
		const Vertex<VIT>* tv = tree.insertVertex(gv->getItem());

		vertices[i] = {
			tv, nullptr,
			gv == start ? zero_cost : max_cost  // w. początkowy ma wagę 0, pozostałe - "nieskończoność"
		};
	}

	// Pobierz tablicę krawędzi
	const auto graph_edges = graph.edges();
	std::size_t num_edges = graph_edges.size();

	// Utwórz tablicę krawędzi specjalnych
	BFEdge<VIT, EIT>* edges = new BFEdge<VIT, EIT>[num_edges];

	// Dodaj krawędzie grafu do tablicy
	for (std::size_t i = 0; i < num_edges; i++) {
		const Edge<VIT, EIT>* ge = graph_edges[i];

		edges[i] = {
			vertices + dynamic_cast<const IndexableVertex<VIT, EIT>*>(ge->getV())->getIndex(),
			vertices + dynamic_cast<const IndexableVertex<VIT, EIT>*>(ge->getW())->getIndex(),
			ge->getItem()
		};
	}

	// Wykonaj relaksację krawędzi
	for (std::size_t i = 1; i < num_vertices; i++) {
		for (std::size_t j = 0; j < num_edges; j++) {
			BFEdge<VIT, EIT>& e = edges[j];
			BFVertex<VIT, EIT>* min_v = e.v;
			BFVertex<VIT, EIT>* max_v = e.w;

			// Wybierz min_v i max_v
			if (max_v->cost < min_v->cost) {
				min_v = e.w;
				max_v = e.v;
			}

			// Jeśli waga wierzchołka min_v nie jest "nieskończonością"
			if (min_v->cost != max_cost) {
				EIT new_cost = min_v->cost + e.cost;

				// Jeśli nowa droga jest krótsza
				if (new_cost < max_v->cost) {
					// Zaktualizuj max_v
					max_v->cost = new_cost;
					max_v->predecessor = min_v;
				}
			}
		}
	}

	// Zapisz krawędzie do drzewa
	for (std::size_t i = 0; i < num_vertices; i++) {
		const BFVertex<VIT, EIT>& v = vertices[i];
		if (v.predecessor)
			tree.insertEdge(v.tree_vertex, v.predecessor->tree_vertex, v.cost - v.predecessor->cost);
	}

	delete[] edges;
	delete[] vertices;
}

template void bellman_ford(const IVGraph<int, int>&, IVGraph<int, int>&, const Vertex<int>*, int, int);