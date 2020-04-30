#pragma once

#include "inc/Base/Edge.hpp"

template <typename VIT, typename EIT>
class AMGraph;

template <typename VIT, typename EIT>
class AMEdge: public Edge<VIT, EIT> {
	friend class AMGraph<VIT, EIT>;

protected:
	AMEdge(Vertex<VIT>* v, Vertex<VIT>* w, EIT value);
};