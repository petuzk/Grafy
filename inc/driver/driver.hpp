#pragma once

#include <vector>
#include <chrono>
#include <limits>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>  // std::find
#include <unistd.h>

#include "inc/Algorithm/util.hpp"

#include "inc/driver/Dataset.hpp"
#include "inc/driver/Directory.hpp"
#include "inc/driver/DatasetGenerator.hpp"

#include "inc/AdjacencyList/ALGraph.hpp"
#include "inc/AdjacencyMatrix/AMGraph.hpp"

#include "inc/Algorithm/dijkstra.hpp"
#include "inc/Algorithm/bellman_ford.hpp"

typedef std::chrono::nanoseconds TimeUnit;
typedef void (* AlgoFunc)(const IVGraph<int, int>&, IVGraph<int, int>&, const Vertex<int>*, int, int);

struct ProgramArgs {
	// Wygeneruj nowy zbiór danych testowych, nadpisz jeżeli istnieje
	bool regenerate_dataset;  /* -g */

	// Przeprowadź test pokazujący działanie podstawowych metod grafu
	bool demo;                /* -d */

	// Przeprowadź test na małej ilości danych
	bool short_test;          /* -s */

	// Przeprowadź test efektywności i zapisz wyniki w formacie CSV do pliku o podanej nazwie.
	// Jeżeli plik już istnieje i zawiera wyniki przerwanego testu, testy na odpowiednich danych
	// nie będą przeprowadzone. Jeżeli plik nie istnieje, będzie utworzony nowy.
	std::string filename;     /* -f */
};

void parse_args(int argc, char* const argv[], ProgramArgs& pa);

struct TestResult {
	// Nazwa pliku z danymi wejściowymi
	std::string filename;

	// Liczba wierzchołków
	std::size_t num_vertices;

	// Gęstość grafu
	double density_percent;

	// Wyniki testu dla poszczególnych implementacji grafu i algorytmów
	TimeUnit::rep AdjMat_Dijkstra;
	TimeUnit::rep AdjMat_BellmanFord;
	TimeUnit::rep AdjList_Dijkstra;
	TimeUnit::rep AdjList_BellmanFord;
};

std::ostream& operator<<(std::ostream& stream, const TestResult& tr);
std::istream& operator>>(std::istream& stream, TestResult& tr);

void print_usage(const char* progname, std::ostream& stream = std::cout);
void print_help(const char* progname, std::ostream& stream = std::cout);
void print_err(const char* progname, const char* message);

class ExitRequired: public std::exception {
public:
	enum DoPrintUsage {
		// Nie wyświetlać instrukcję wywołania
		NO_PRINT,

		// Wyświetlać instrukcję przed opisem błedu lub w przypadku go braku
		PRINT_BEFORE_MSG,

		// Wyświetlać instrukcję po opisie błedu
		PRINT_AFTER_MSG
	};

private:
	const char* message;
	int exit_code;
	DoPrintUsage do_print_usage;

public:
	ExitRequired(int exit_code, const char* message = nullptr, DoPrintUsage usage = NO_PRINT):
		exit_code(exit_code), message(message), do_print_usage(usage) { }

	const char* what() const noexcept override { return message; }
	int exitCode() const { return exit_code; }
	DoPrintUsage doPrintUsage() const { return do_print_usage; }

	int print(const char* progname) const {
		if (doPrintUsage() == PRINT_BEFORE_MSG)
			print_usage(progname, std::cerr);

		if (what()) {
			print_err(progname, what());
			if (doPrintUsage() == PRINT_AFTER_MSG)
				print_usage(progname, std::cerr);
		}

		return exitCode();
	}
};

void prepare_dataset(Directory& dataset_dir, bool force_gen = false);

void demo_on_abstract(AbstractGraph<int, int>& graph);
void demo();

void write_graph(std::ostream& stream, const AbstractGraph<int, int>& tree, const Vertex<int>* start);
template <typename Graph>
void test_and_write_tree(
	const Graph& graph,
	const Vertex<int>* start_v,
	AlgoFunc af,
	const std::string& alg_name,
	const std::string& graph_impl,
	const std::string& filename);
void short_test(Directory& test_dir);

template <typename Graph>
TimeUnit measure(const Graph& graph, const Vertex<int>* start_v, AlgoFunc af);
void test(Directory& dataset_dir, const std::string& filename);