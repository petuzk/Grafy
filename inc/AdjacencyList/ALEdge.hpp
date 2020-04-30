#pragma once

#include "inc/Base/Edge.hpp"
#include "inc/Container/LinkedList.hpp"

template <typename VIT, typename EIT>
class ALGraph;

template <typename VIT, typename EIT>
class ALEdge: public Edge<VIT, EIT> {
	friend class ALGraph<VIT, EIT>;

protected:
	unsigned char flag;
	typename LinkedList<Edge<VIT, EIT>*>::Element* v_llelem;
	typename LinkedList<Edge<VIT, EIT>*>::Element* w_llelem;

	ALEdge(Vertex<VIT>* v, Vertex<VIT>* w, EIT value, unsigned char flag);
};