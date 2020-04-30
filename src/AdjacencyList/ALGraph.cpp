#include "inc/AdjacencyList/ALGraph.hpp"

template <typename VIT, typename EIT>
ALGraph<VIT, EIT>::ALGraph(): edges_count(0) { }

template <typename VIT, typename EIT>
ALGraph<VIT, EIT>::~ALGraph() {
	for (const Edge<VIT, EIT>* edge: edges()) {
			delete edge;
	}
}

template <typename VIT, typename EIT>
unsigned char ALGraph<VIT, EIT>::unvisitedEdgeFlagValue() const {
	for (LinkedList<Edge<VIT, EIT>*>& ll: connections) {
		for (auto ll_elem: ll) {
			return dynamic_cast<ALEdge<VIT, EIT>*>(ll_elem->getItem())->flag;
		}
	}

	return 0;
}

template <typename VIT, typename EIT>
unsigned char ALGraph<VIT, EIT>::visitedEdgeFlagValue(unsigned char unvisited) const {
	return unvisited + 1;
}

template <typename VIT, typename EIT>
const DynamicArray<const Edge<VIT, EIT>*> ALGraph<VIT, EIT>::edges() const {
	unsigned char unvis = unvisitedEdgeFlagValue(), vis = visitedEdgeFlagValue(unvis);
	DynamicArray<const Edge<VIT, EIT>*> arr(edges_count);

	for (LinkedList<Edge<VIT, EIT>*>& ll: connections) {
		for (auto ll_elem: ll) {
			Edge<VIT, EIT>* edge = ll_elem->getItem();
			ALEdge<VIT, EIT>* aledge = dynamic_cast<ALEdge<VIT, EIT>*>(edge);
			if (aledge->flag == unvis) {
				arr.add(edge);
				aledge->flag = vis;
			}
		}
	}

	return arr;
}

template <typename VIT, typename EIT>
const DynamicArray<const Edge<VIT, EIT>*> ALGraph<VIT, EIT>::incidentEdges(const Vertex<VIT>* v) const {
	const LinkedList<Edge<VIT, EIT>*>& ll = connections[IVGraph<VIT, EIT>::getVertexIndex(v)];
	DynamicArray<const Edge<VIT, EIT>*> arr;

	for (auto ll_elem: ll) {
		Edge<VIT, EIT>* edge = ll_elem->getItem();
		arr.add(edge);
	}

	return arr;
}

template <typename VIT, typename EIT>
const Edge<VIT, EIT>* ALGraph<VIT, EIT>::edgeBetween(const Vertex<VIT>* v, const Vertex<VIT>* w) const {
	const LinkedList<Edge<VIT, EIT>*>& vlist = connections[IVGraph<VIT, EIT>::getVertexIndex(v)];
	const LinkedList<Edge<VIT, EIT>*>& wlist = connections[IVGraph<VIT, EIT>::getVertexIndex(w)];
	const LinkedList<Edge<VIT, EIT>*>& ll = (vlist.size() < wlist.size()) ? vlist : wlist;

	for (auto ll_elem: ll) {
		Edge<VIT, EIT>* edge = ll_elem->getItem();
		if (edge->getOpposite(v) == w)
			return edge;
	}

	return nullptr;
}

template <typename VIT, typename EIT>
Vertex<VIT>* ALGraph<VIT, EIT>::insertVertex(VIT value) {
	bool reused;
	Vertex<VIT>* v = IVGraph<VIT, EIT>::insertIVertex(value, reused);

	if (!reused)
		connections.add(LinkedList<Edge<VIT, EIT>*>());

	return v;
}

template <typename VIT, typename EIT>
Edge<VIT, EIT>* ALGraph<VIT, EIT>::insertEdge(const Vertex<VIT>* v, const Vertex<VIT>* w, EIT value) {
	if (edgeBetween(v, w))
		throw std::logic_error("edge already exists");

	Vertex<VIT>* ncv = const_cast<Vertex<VIT>*>(v);
	Vertex<VIT>* ncw = const_cast<Vertex<VIT>*>(w);

	edges_count++;
	auto edge = new ALEdge<VIT, EIT>(ncv, ncw, value, unvisitedEdgeFlagValue());
	auto v_ll = connections[IVGraph<VIT, EIT>::getVertexIndex(v)].insertAfter(edge);
	auto w_ll = connections[IVGraph<VIT, EIT>::getVertexIndex(w)].insertAfter(edge);
	edge->v_llelem = v_ll;
	edge->w_llelem = w_ll;

	return edge;
}

template <typename VIT, typename EIT>
void ALGraph<VIT, EIT>::removeVertex(const Vertex<VIT>* v) {
	std::size_t v_idx;
	IVGraph<VIT, EIT>::removeIVertex(v, v_idx);
	connections[v_idx].clear();
}

template <typename VIT, typename EIT>
void ALGraph<VIT, EIT>::removeEdge(const Edge<VIT, EIT>* edge) {
	edges_count--;
	const ALEdge<VIT, EIT>* aledge = dynamic_cast<const ALEdge<VIT, EIT>*>(edge);
	aledge->v_llelem->getLL()->remove(aledge->v_llelem);
	aledge->w_llelem->getLL()->remove(aledge->w_llelem);
}

template <typename VIT, typename EIT>
void ALGraph<VIT, EIT>::preallocateForVertices(std::size_t count) {
	IVGraph<VIT, EIT>::preallocateForVertices(count);
	connections.allocate(count);
}

template <typename VIT, typename EIT>
void ALGraph<VIT, EIT>::preallocateForEdges(std::size_t count) { /* do nothing */ }

template class ALGraph<int, int>;
