#pragma once

#include <fstream>

#include "inc/Base/Vertex.hpp"
#include "inc/Base/Edge.hpp"
#include "inc/Container/DynamicArray.hpp"

template <typename VIT, typename EIT>
class AbstractGraph {
public:

	// Zwraca tablicę wszystkich wierzchołków
	virtual const DynamicArray<const Vertex<VIT>*> vertices() const = 0;

	// Zwraca tablicę wszystkich krawędzi
	virtual const DynamicArray<const Edge<VIT, EIT>*> edges() const = 0;

	// Zwraca tablicę krawędzi incydentnych do wierzchołka v
	virtual const DynamicArray<const Edge<VIT, EIT>*> incidentEdges(const Vertex<VIT>* v) const = 0;

	// Zwraca krawędź połączającą wierzchołki
	virtual const Edge<VIT, EIT>* edgeBetween(const Vertex<VIT>* v, const Vertex<VIT>* w) const = 0;



	// Te metody wywoływają powyższe, ale nie są const i zwracają modyfikowalne krawędzie i wierzchołki
	virtual const DynamicArray<Vertex<VIT>*> vertices();
	virtual const DynamicArray<Edge<VIT, EIT>*> edges();
	virtual const DynamicArray<Edge<VIT, EIT>*> incidentEdges(const Vertex<VIT>* v);
	virtual Edge<VIT, EIT>* edgeBetween(const Vertex<VIT>* v, const Vertex<VIT>* w);



	// Wstawia wierzchołek przechowujący podaną wartość
	virtual Vertex<VIT>* insertVertex(VIT value) = 0;

	// Wstawia krawędź połączającą wierzchołki v i w i przechowującą podaną wartość
	virtual Edge<VIT, EIT>* insertEdge(const Vertex<VIT>* v, const Vertex<VIT>* w, EIT value) = 0;

	// Usuwa wierzchołek
	virtual void removeVertex(const Vertex<VIT>* v) = 0;

	// Usuwa krawędź
	virtual void removeEdge(const Edge<VIT, EIT>* e) = 0;



	// Poniższe metody alokują pamięć dla przechowywania podanej liczby wierzchołków / krawędzi
	// Mają być wywoływane na grafie pustym
	virtual void preallocateForVertices(std::size_t count) = 0;
	virtual void preallocateForEdges(std::size_t count) = 0;
};