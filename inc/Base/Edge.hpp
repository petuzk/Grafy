#pragma once

#include <ostream>
#include "inc/Base/Cell.hpp"
#include "inc/Base/Vertex.hpp"

template <typename VIT, typename EIT>
class Edge: public Cell<EIT> {
	Vertex<VIT>* v;
	Vertex<VIT>* w;

protected:
	void setV(Vertex<VIT>* x);
	void setW(Vertex<VIT>* x);
	void setOpposite(const Vertex<VIT>* opp_to, Vertex<VIT>* x);

	Edge(Vertex<VIT>* v, Vertex<VIT>* w, EIT value);  // może być utworzony tylko za pomocą odpowiedniej metody grafa

public:
	typedef EIT T;

	virtual ~Edge() = default;

	Vertex<VIT>* getV();
	Vertex<VIT>* getW();
	Vertex<VIT>* getOpposite(const Vertex<VIT>* opp_to);

	const Vertex<VIT>* getV() const;
	const Vertex<VIT>* getW() const;
	const Vertex<VIT>* getOpposite(const Vertex<VIT>* opp_to) const;
};

template <typename VIT, typename EIT>
std::ostream& operator<<(std::ostream& stream, const Edge<VIT, EIT>* v);