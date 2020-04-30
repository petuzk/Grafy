#include "inc/driver/DatasetGenerator.hpp"

/*
 * Porównuje grupy pierwsze
 */

bool DatasetGenerator::FirstGroup::operator==(const DatasetGenerator::FirstGroup& other) const {
	return from == other.from && to == other.to;
}

/*
 * Porównuje krawędzie nieskierowane i zwraca true gdy są równoległe
 */

bool DatasetGenerator::Edge::operator==(const DatasetGenerator::Edge& other) const {
	return (from == other.from && to == other.to) || (from == other.to && to == other.from);
}

/*
 * Zapisuje krawędź do podanego strumienia
 */

std::ostream& operator<<(std::ostream& stream, const DatasetGenerator::Edge& e) {
	return stream << e.from << ' ' << e.to << ' ' << pickRand(0, 100) << '\n';
}

/*
 * Zwraca false jeżeli krawędź może być wykorzystana, tzn.:
 *   - krawędź nie jest cyklem
 *   - krawędź nie łączy wierzchołki z grupy pierwszej
 *   - krawędź nie została już wykorzystana
 */

bool invalidEdge(
	const DatasetGenerator::Edge& edge,
	std::size_t num_vertices,
	const DynamicArray<DatasetGenerator::Edge>& edges,
	const DynamicArray<DatasetGenerator::FirstGroup>& firstgroups)
{
	std::size_t min, max;

	if (edge.to == edge.from) {
		// Krawędź jest cyklem
		return true;
	} else if (edge.to > edge.from) {
		max = edge.to; min = edge.from;
	} else {
		min = edge.to; max = edge.from;
	}

	if (min + 1 == max) {
		// Krawędź łączy wierzchołki o sąsiednich numerach, więc trzeba sprawdzić, czy nie są z grupy pierwszej
		for (const DatasetGenerator::FirstGroup& group: firstgroups) {
			if (min >= group.from && max < group.to)
				return true;
			else if (max <= group.from)
				break;
		}
	}

	// Jeżeli krawędź już została wykorzystana, to porównanie zwróci true
	return edges.find(edge) < edges.size();
}

void DatasetGenerator::generate(std::size_t num_vertices, double density, const std::string& save_path) {
	std::ofstream file(save_path);
	generate(num_vertices, density, file);
	file.close();
}

/*
 * Generuje graf o określonej liczbie wierzchołków i gęstości i zapisuje do strumiania.
 * Algorytm jest następujący:
 *   1. Najpierw generowane są "grupy pierwsze". Są to wierzchołki połączone szeregowo, np.:
 *       v1 - v2 - v3   v4 - v5   v6 - v7   v8
 *      Typowo rozmiar grupy jest od 2 wierzchołków do 1/3 od liczby wierzchołków. Może powstać grupa
 *      jednoelementowa na końcu.
 *   2. Wierzchołki z grup pierwszych są łączone między sobą.
 *   3. Z każdej grupy pierwszej jest losowany jeden wierzchołek i łączony z wylosowanym z kolejnej grupy.
 *   4. Tworzona jest tablica wszystkich możliwych krawędzi. W przypadku, gdy krawędź nie spełnia
 *      kryteriów określonych w opisie invalidEdge(), jest odrazu oznaczana jako użyta.
 *   5. Z tej tablicy losowane są krawędzi, zapisywane do strumienia i oznaczane jako użyte.
 */

void DatasetGenerator::generate(std::size_t num_vertices, double density, std::ostream& stream) {
	if (density <= 0 || density > 1) {
		throw std::out_of_range("density");
	}

	std::srand(std::time(0));
	std::size_t num_edges = density * num_vertices * (num_vertices - 1) / 2;
	std::size_t max_edges = num_vertices * (num_vertices - 1) / 2;

	if (num_edges < num_vertices - 2) {
		throw std::out_of_range("density is too low, can't connect all vertices");
	}
	else if (num_edges > max_edges) {
		// Na wszelki przypadek ubezpieczam się od niedokładności obliczeń
		num_edges = max_edges;
	}

	// Zapisz pierwszą linię
	stream << num_edges << ' ' << num_vertices << ' ' << (std::rand() % num_vertices) << '\n';

	if (max_edges - (num_vertices - 1) < num_edges) {
		return generateDense(num_vertices, max_edges - num_edges, stream);
	}

	// Wygeneruj grupy pierwsze
	std::size_t from = 0, to;
	DynamicArray<FirstGroup> firstgroups;

	while (from != num_vertices) {
		to = from + pickRand(2, num_vertices / 3 + 2);
		if (to > num_vertices)
			to = num_vertices;

		firstgroups.add({ from, to });
		from = to;
	}

	// Połącz wierzchołki z grup
	for (const FirstGroup& group: firstgroups) {
		for (std::size_t i = group.from + 1; i < group.to; i++) {
			stream << Edge { i-1, i };
		}
	}

	// Połącz grupy między sobą
	DynamicArray<Edge> edges_step2(firstgroups.size() - 1);
	edges_step2.setSize(firstgroups.size() - 1);

	for (std::size_t i = 0; i < firstgroups.size() - 1; i++) {
		const FirstGroup& fromgrp = firstgroups[i];
		const FirstGroup& togrp = firstgroups[i + 1];

		edges_step2[i] = {
			pickRand(fromgrp.from, fromgrp.to),
			pickRand(togrp.from, togrp.to)
		};

		stream << edges_step2[i];
	}

	// Generuj tablicę dostępnych krawędzi
	/*
	 * Tu teoretycznie można zmniejszyć zużycie pamięci jeśli nie generować krawędzie,
	 * które później będą oznaczone jako użyte (czyli już są użyte), ale nie jest ich
	 * dużo w porównaniu do max_edges.
	 * Choć z tej tablicy większość może być wygenerowana bezużytecznie, ta wersja jest lepsza,
	 * niż poprzednia, w której losowano krawędź dopóki nie spełni kryteriów określonych w opisie
	 * invalidEdge() i później dodawano ją do tablicy użytych krawędzi, gdyż w takim przypadku
	 * przeszukiwanie krawędzi w tablicy (w invalidEdge()) zajmuje coraz więcej czasu.
	 */

	Edge* edges_step3 = new Edge[max_edges];

	for (std::size_t i = 1; i < num_vertices; i++) {
		for (std::size_t j = 0; j < i; j++) {
			std::size_t idx = triangular_number(i - 1) + j;
			edges_step3[idx] = { i, j };
			if (invalidEdge(edges_step3[idx], num_vertices, edges_step2, firstgroups)) {
				// Oznacz krawędź jako użyta
				edges_step3[idx].from = num_vertices;
			}
		}
	}

	// Wylosuj krawędzie
	for (std::size_t i = num_vertices - 1; i < num_edges; ) {
		Edge& edge = edges_step3[std::rand() % max_edges];
		if (edge.from != num_vertices) {
			stream << edge;
			edge.from = num_vertices;
			i++;
		}
	}

	delete[] edges_step3;
}

/*
 * Generuje graf o wysokiej gęstości, czyli takiej, że nie istnieje kombinacja krawędzi, dla której
 * graf będzie niespójny. Algorytm można stosować, gdy liczba krawędzi jest większa niż
 * (V - 1)^2, gdzie V to liczba wierzchołków. Argument pop_count jest różnicą maksymalnej liczby krawędzi
 * (V * (V - 1)) i żądanej.
 * Algorytm jest następujący:
 *   1. Generowana jest tablica wszystkich możliwych krawędzi.
 *   2. Z tej tablicy pop_count losowych wierchołków są oznaczane jako niepotrzebne.
 *   3. Każda nieoznaczona krawędz jest zapisywana do strumienia.
 */

void DatasetGenerator::generateDense(std::size_t num_vertices, std::size_t pop_count, std::ostream& stream) {
	if (pop_count >= num_vertices - 1)
		throw std::out_of_range("pop_count");

	std::size_t i, j, max_edges = num_vertices * (num_vertices - 1) / 2;
	Edge* edges = new Edge[max_edges];

	// Generuj tablicę krawędzi
	for (i = 1; i < num_vertices; i++) {
		for (j = 0; j < i; j++) {
			edges[triangular_number(i - 1) + j] = { i, j };
		}
	}

	// Oznacz niepotrzebne krawędzie
	for (i = 0; i < pop_count; ) {
		Edge& edge = edges[std::rand() % max_edges];
		if (edge.from != num_vertices) {
			edge.from = num_vertices;
			i++;
		}
	}

	// Zapisz
	for (i = 0; i < max_edges; i++) {
		if (edges[i].from != num_vertices)
			stream << edges[i];
	}

	delete[] edges;
}