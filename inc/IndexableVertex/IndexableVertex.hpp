#pragma once

#include <cstdlib>
#include "inc/Base/Vertex.hpp"

template <typename VIT, typename EIT>
class IVGraph;

template <typename VIT, typename EIT>
class IndexableVertex: public Vertex<VIT> {
	friend class IVGraph<VIT, EIT>;

protected:
	std::size_t index;

	IndexableVertex(std::size_t index, VIT item);

public:
	std::size_t getIndex() const;
};