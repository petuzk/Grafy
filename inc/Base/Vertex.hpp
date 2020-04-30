#pragma once

#include <ostream>
#include "inc/Base/Cell.hpp"

template <typename VIT>
class Vertex: public Cell<VIT> {
protected:
	Vertex(VIT item);  // może być utworzony tylko za pomocą odpowiedniej metody grafa

public:
	typedef VIT T;

	virtual ~Vertex() = default;
};

template <typename VIT>
std::ostream& operator<<(std::ostream& stream, const Vertex<VIT>* v);