#include "inc/Algorithm/dijkstra.hpp"

template <typename VIT, typename EIT>
void dijkstra(const IVGraph<VIT, EIT>& graph, IVGraph<VIT, EIT>& tree, const Vertex<VIT>* start, EIT zero_cost, EIT max_cost) {
	const auto vertices = graph.vertices();
	// Liczba wierzchołków, równocześnie rozmiar tablicy je przechowującej
	std::size_t unvisited_size = vertices.size();
	tree.preallocateForVertices(unvisited_size);
	tree.preallocateForEdges(unvisited_size - 1);

	// Tablica niezwiedzonych wierzchołków
	VertexWithCost<VIT, EIT>* unvisited = new VertexWithCost<VIT, EIT>[unvisited_size];  // będzie iterowana
	VertexWithCost<VIT, EIT>* const unvisited_start = unvisited;  // służy do zwolnienia pamięci na końcu

	// Dodaj wierzchołki do tablicy
	for (std::size_t i = 0; i < unvisited_size; i++) {
		const Vertex<VIT>* gv = vertices[i];

		// Wstaw najbliższy wierzchołek do drzewa
		const Vertex<VIT>* tv = tree.insertVertex(gv->getItem());

		unvisited[i] = {
			gv, tv, nullptr,
			gv == start ? zero_cost : max_cost  // w. początkowy ma wagę 0, pozostałe - "nieskończoność"
		};
	}

	// Dopóki są niezwiedzone wierzchołki, wykonaj { ... } i przesuń początek tablicy
	for (; unvisited_size; unvisited++, unvisited_size--) {
		// Posortuj wierzchołki, aby na początku był najbliższy
		heapsort(unvisited, unvisited_size);

		VertexWithCost<VIT, EIT>* predecessor = unvisited->predecessor;
		// Jeżeli ma "poprzednika"
		if (predecessor) {
			// Połącz je
			tree.insertEdge(predecessor->tree_vertex, unvisited->tree_vertex, unvisited->cost - predecessor->cost);
		}

		// Zaktualizuj odległości do sąsiadów
		for (std::size_t i = 1; i < unvisited_size; i++) {
			VertexWithCost<VIT, EIT>* update = unvisited + i;
			const Edge<VIT, EIT>* e = graph.edgeBetween(unvisited->graph_vertex, update->graph_vertex);

			// Jeżeli wierzchołki są sąsiednie, e jest krawędzią je połączającą
			if (e) {
				// Oblicz nową wagę
				EIT new_cost = unvisited->cost + e->getItem();

				// Jeżeli jest mniejsza niż obecna, zapisz ję i zmień "poprzednika"
				if (new_cost < update->cost) {
					update->cost = new_cost;
					update->predecessor = unvisited;
				}
			}
		}
	}

	// Zwolnij pamięć
	delete[] unvisited_start;
}

template struct VertexWithCost<int, int>;
template void dijkstra(const IVGraph<int, int>&, IVGraph<int, int>&, const Vertex<int>*, int, int);