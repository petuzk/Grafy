#pragma once

#include <ctime>
#include <cstdlib>
#include <fstream>
#include "inc/Algorithm/util.hpp"
#include "inc/Container/DynamicArray.hpp"

/*
 * Algorytm jest opisany w pliku DatasetGenerator.cpp
 * przy metodzie DatasetGenerator::generate()
 */

class DatasetGenerator {
public:
	struct FirstGroup {
		std::size_t from;
		std::size_t to;

		bool operator==(const FirstGroup& other) const;
	};

	struct Edge {
		std::size_t from;
		std::size_t to;

		bool operator==(const Edge& other) const;
	};

	static void generate(std::size_t num_vertices, double density, const std::string& save_path);
	static void generate(std::size_t num_vertices, double density, std::ostream& stream);

private:
	static void generateDense(std::size_t num_vertices, std::size_t pop_count, std::ostream& stream);
};