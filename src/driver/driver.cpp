#include "inc/driver/driver.hpp"

void parse_args(int argc, char* const argv[], ProgramArgs& pa) {
	int opt;

	pa.regenerate_dataset = false;
	pa.demo = false;
	pa.short_test = false;
	pa.filename = "";

	while ((opt = getopt(argc, argv, "gdsf:h?")) != -1) {
		switch (opt) {
			case 'g':
				pa.regenerate_dataset = true;
				break;
			case 'd':
				pa.demo = true;
				break;
			case 's':
				pa.short_test = true;
				break;
			case 'f':
				pa.filename = optarg;
				break;
			case 'h':
				print_help(argv[0]);
				throw ExitRequired(0);
			case '?':
				print_usage(argv[0]);
				throw ExitRequired(0);
			default:
				throw ExitRequired(1, "getopt error");
		}
	}

	if (!(pa.regenerate_dataset || pa.demo || pa.short_test || pa.filename.length())) {
		throw ExitRequired(0, "no tasks to be done", ExitRequired::PRINT_AFTER_MSG);
	}
}

std::ostream& operator<<(std::ostream& stream, const TestResult& tr) {
	return stream << tr.filename << ", "
	              << tr.num_vertices << ", "
	              << tr.density_percent << ", "
	              << tr.AdjMat_Dijkstra << ", "
	              << tr.AdjMat_BellmanFord << ", "
	              << tr.AdjList_Dijkstra << ", "
	              << tr.AdjList_BellmanFord << '\n';
}

std::istream& operator>>(std::istream& stream, TestResult& tr) {
	char comma;

	stream >> tr.filename;
	comma = tr.filename.back();
	tr.filename.pop_back();
	if (comma != ',')   stream.setstate(std::ios::failbit);
	if (!stream.good()) return stream;

	stream >> tr.num_vertices >> comma;
	if (comma != ',')   stream.setstate(std::ios::failbit);
	if (!stream.good()) return stream;

	stream >> tr.density_percent >> comma;
	if (comma != ',')   stream.setstate(std::ios::failbit);
	if (!stream.good()) return stream;

	stream >> tr.AdjMat_Dijkstra >> comma;
	if (comma != ',')   stream.setstate(std::ios::failbit);
	if (!stream.good()) return stream;

	stream >> tr.AdjMat_BellmanFord >> comma;
	if (comma != ',')   stream.setstate(std::ios::failbit);
	if (!stream.good()) return stream;

	stream >> tr.AdjList_Dijkstra >> comma;
	if (comma != ',')   stream.setstate(std::ios::failbit);
	if (!stream.good()) return stream;

	stream >> tr.AdjList_BellmanFord;

	return stream;
}

void print_usage(const char* progname, std::ostream& stream) {
	stream << "usage: "
	       << progname
	       << " -h | ( [-g] [-d] [-s] [-f filename] )"
	       << std::endl;
}

void print_help(const char* progname, std::ostream& stream) {
	print_usage(progname, stream);

	stream << std::endl;
	stream << "Ten program testuje algorytmy Dijkstry i Bellmana-Forda na grafach" << std::endl;
	stream << "implementowanych w postaci listy sąsiedztwa i macierzy sąsiedztwa." << std::endl;
	stream << "Grafy są generowane losowo i są spójne, bez cyklów i krawędzi ujemnych." << std::endl;
	stream << "Wyniki testów (czas w ns) są zapisywane do pliku CSV o podanej nazwie." << std::endl;
	stream << std::endl;
	stream << "Przełączniki" << std::endl;
	stream << "    -g  Wygeneruj nowy zbiór danych testowych, nadpisz jeżeli istnieje." << std::endl;
	stream << "    -d  Przeprowadź test pokazujący działanie podstawowych metod grafu." << std::endl;
	stream << "    -s  Przeprowadź test na małej liczbie danych losowych i zapisz wyniki." << std::endl;
	stream << "    -f  Przeprowadź test na wcześniej wygenerownych danych." << std::endl;
	stream << "        Jeżeli dane nie są jeszcze wygenerowane, wygeneruj nowe." << std::endl;
	stream << "        Jeżeli plik CSV już istnieje i zawiera wyniki przerwanego testu," << std::endl;
	stream << "        testy na odpowiednich danych nie będą przeprowadzone." << std::endl;
	stream << "        Jeżeli plik nie istnieje, będzie utworzony nowy." << std::endl;
	stream << "    -h  Pokaż help." << std::endl;
}

void print_err(const char* progname, const char* message) {
	std::cerr << progname << ": " << message << std::endl;
}

void prepare_dataset(Directory& dataset_dir, bool force_gen) {
	auto status = dataset_dir.status();
	if (status == Directory::NOT_A_DIRECTORY)
		throw ExitRequired(10, "dataset path is not a directory");
	else if (status == Directory::EXISTS) {
		if (!force_gen)
			return;
		else
			dataset_dir.removeAll();
	}

	std::cout << "Generuję dane...";
	std::cout.flush();

	dataset_dir.create();

	std::size_t each = 100;
	std::vector<std::size_t> nums_vertices = {10, 50, 100, 500, 1000};
	std::vector<double> densities = {25, 50, 75, 100};

	for (auto num_vertices: nums_vertices)
	for (auto density: densities) {
		std::ostringstream stream;
		stream << num_vertices << '_' << density;
		Directory dir(dataset_dir.sub(stream.str()));
		dir.create();

		for (int i = 0; i < each; i++)
			DatasetGenerator::generate(num_vertices, density / 100, dir.sub(std::to_string(i+1) + ".dataset"));

		std::cout << '.';
		std::cout.flush();
	}

	std::cout << " Gotowe!" << std::endl;
}

void demo_on_abstract(AbstractGraph<int, int>& graph) {
	Vertex<int> *u, *v, *w;
	Edge<int, int> *a, *b;

	std::cout << bold << "Dodaje wierzchołki do grafu:" << normal << std::endl;
	std::cout << "u = graph.insertVertex(20) = " << (u = graph.insertVertex(20)) << std::endl;
	std::cout << "v = graph.insertVertex(15) = " << (v = graph.insertVertex(15)) << std::endl;
	std::cout << "w = graph.insertVertex(30) = " << (w = graph.insertVertex(30)) << std::endl;

	std::cout << "Wierzchołki grafu (z graph.vertices()):" << std::endl;
	for (const Vertex<int>* x: graph.vertices())
		std::cout << "  - " << x << std::endl;

	std::cout << std::endl;

	std::cout << bold << "Dodaje krawędzie do grafu:" << normal << std::endl;
	std::cout << "a = graph.insertEdge(u, v, 1) = " << (a = graph.insertEdge(u, v, 1)) << std::endl;
	std::cout << "b = graph.insertEdge(v, w, 2) = " << (b = graph.insertEdge(v, w, 2)) << std::endl;

	std::cout << "Krawędzie grafu (z graph.edges()):" << std::endl;
	for (const Edge<int, int>* x: graph.edges())
		std::cout << "  - " << x << std::endl;

	std::cout << std::endl;

	std::cout << bold << "Inne operacje na grafie:" << normal << std::endl;
	v->setItem(10);
	std::cout << "v.setItem(10); v = " << v << std::endl;
	std::cout << "a.getOpposite(v) = " << a->getOpposite(v) << std::endl;

	std::cout << "graph.edgeBetween(v, w) = " << graph.edgeBetween(v, w) << std::endl;
	std::cout << "graph.edgeBetween(u, w) = " << graph.edgeBetween(u, w) << std::endl;

	std::cout << "Krawędzie przylegające do v (z graph.incidentEdges(v)):" << std::endl;
	for (const Edge<int, int>* x: graph.incidentEdges(v))
		std::cout << "  - " << x << std::endl;

	std::cout << std::endl;

	std::cout << bold << "Usuwam elementy grafu:" << normal << std::endl;
	std::cout << "graph.removeEdge(b)" << std::endl; graph.removeEdge(b);
	std::cout << "graph.removeVertex(w)" << std::endl; graph.removeVertex(w);

	std::cout << "Wierzchołki grafu (z graph.vertices()):" << std::endl;
	for (const Vertex<int>* x: graph.vertices())
		std::cout << "  - " << x << std::endl;

	std::cout << "Krawędzie grafu (z graph.edges()):" << std::endl;
	for (const Edge<int, int>* x: graph.edges())
		std::cout << "  - " << x << std::endl;
}

void demo() {
	AMGraph<int, int> am;
	ALGraph<int, int> al;
	std::cout << "[TEST NA GRAFIE W POSTACI MACIERZY SĄSIEDZTWA]" << std::endl;
	demo_on_abstract(am);
	std::cout << std::endl << std::endl;
	std::cout << "[TEST NA GRAFIE W POSTACI LIŚCIE SĄSIEDZTWA]" << std::endl;
	demo_on_abstract(al);
}

void write_graph(std::ostream& stream, const AbstractGraph<int, int>& tree, const Vertex<int>* start) {
	const auto edges = tree.edges();
	stream << edges.size() << ' ' << tree.vertices().size() << ' ' << start->getItem() << '\n';

	for (const Edge<int, int>* edge: edges)
		stream << edge->getV()->getItem() << ' ' << edge->getW()->getItem() << ' ' << edge->getItem() << '\n';
}

template <typename Graph>
void test_and_write_tree(
	const Graph& graph,
	const Vertex<int>* start_v,
	AlgoFunc af,
	const std::string& alg_name,
	const std::string& graph_impl,
	const std::string& filename)
{
	std::cout << "Sprawdzam algorytm " << alg_name
	          << " na grafie na " << graph_impl
	          << " i zapisuję wynik do pliku " << underline << filename << normal << std::endl;

	Graph tree;
	int zero_cost = 0, max_cost = std::numeric_limits<int>::max();
	af(graph, tree, start_v, zero_cost, max_cost);

	std::ofstream file(filename);
	write_graph(file, tree, start_v);
	file.close();
}

void short_test(Directory& test_dir) {
	auto status = test_dir.status();
	if (status == Directory::NOT_A_DIRECTORY)
		throw ExitRequired(11, "test path is not a directory");
	else if (status == Directory::EXISTS)
		test_dir.removeAll();

	test_dir.create();

	std::string filename = test_dir.sub("test.dataset");
	std::cout << "Generuję graf testowy i zapisuję do pliku " << underline << filename << normal << std::endl;
	DatasetGenerator::generate(10, static_cast<double>(pickRand(20, 90)) / 100, filename);
	Dataset* dat = new Dataset(filename);

	test_and_write_tree(
		dat->getAMGraph(), dat->getStartVertexAM(), dijkstra, "Dijkstry", "macierzy sąsiedztwa", test_dir.sub("AdjMat_Dijkstra.result"));
	test_and_write_tree(
		dat->getAMGraph(), dat->getStartVertexAM(), bellman_ford, "Bellmana-Forda", "macierzy sąsiedztwa", test_dir.sub("AdjMat_BellmanFord.result"));
	test_and_write_tree(
		dat->getALGraph(), dat->getStartVertexAL(), dijkstra, "Dijkstry", "liście sąsiedztwa", test_dir.sub("AdjList_Dijkstra.result"));
	test_and_write_tree(
		dat->getALGraph(), dat->getStartVertexAL(), bellman_ford, "Bellmana-Forda", "liście sąsiedztwa", test_dir.sub("AdjList_BellmanFord.result"));

	delete dat;
}

template <typename Graph>
TimeUnit measure(const Graph& graph, const Vertex<int>* start_v, AlgoFunc af) {
	Graph tree;
	std::chrono::steady_clock::time_point start, end;
	int zero_cost = 0, max_cost = std::numeric_limits<int>::max();

	start = std::chrono::steady_clock::now();
	af(graph, tree, start_v, zero_cost, max_cost);
	end = std::chrono::steady_clock::now();

	return std::chrono::duration_cast<TimeUnit>(end - start);
}

void test(Directory& dataset_dir, const std::string& filename) {
	std::vector<std::string> skip_files;
	std::fstream file(filename);

	if (file.is_open()) {
		// Pomiń pierwszą linię
		while (file.good() && file.get() != '\n');
		while (file.good()) {
			TestResult tr;
			file >> tr;
			if (!file.fail())
				skip_files.push_back(tr.filename);
		}
		file.clear();
	}
	else {
		file.clear();
		file.open(filename, std::ios::out);
		file << "Filename, Vertices, Density, AdjMat Dijkstra, AdjMat BellmanFord, AdjList Dijkstra, AdjList BellmanFord\n";
		file.flush();
	}

	std::cout << "Testuję...";
	std::cout.flush();

	auto skip_begin = skip_files.cbegin();
	auto skip_end = skip_files.cend();

	for (std::string filename: dataset_dir.iter("*.dataset")) {
		if (std::find(skip_begin, skip_end, filename) == skip_end) {
			TestResult tr;
			Dataset* dat = new Dataset(filename);

			tr.filename = filename;
			tr.num_vertices = dat->graphNumVertices();
			tr.density_percent = dat->graphDensity() * 100;


			tr.AdjMat_Dijkstra     = 0; //measure(dat->getAMGraph(), dat->getStartVertexAM(), dijkstra).count();
			tr.AdjMat_BellmanFord  = 0; //measure(dat->getAMGraph(), dat->getStartVertexAM(), bellman_ford).count();
			tr.AdjList_Dijkstra    = measure(dat->getALGraph(), dat->getStartVertexAL(), dijkstra).count();
			tr.AdjList_BellmanFord = 0; //measure(dat->getALGraph(), dat->getStartVertexAL(), bellman_ford).count();

			delete dat;

			file << tr;
			file.flush();

			std::cout << '.';
			std::cout.flush();
		}
	}

	std::cout << " Gotowe!" << std::endl;
}

int main(int argc, char* const argv[]) {
	ProgramArgs pa;
	Directory dataset_dir("dataset");
	Directory test_dir("test");

	try {
		parse_args(argc, argv, pa);

		if (pa.demo)
			demo();

		if (pa.short_test)
			short_test(test_dir);

		if (pa.filename.length() || pa.regenerate_dataset)
			prepare_dataset(dataset_dir, pa.regenerate_dataset);

		if (pa.filename.length())
			test(dataset_dir, pa.filename);
	}
	catch (const ExitRequired& err) {
		return err.print(argv[0]);
	}

	return 0;
}