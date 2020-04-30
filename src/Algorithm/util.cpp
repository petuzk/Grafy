#include "inc/Algorithm/util.hpp"

template <typename T>
T triangular_number(T x) {
	if (x < 2)
		return x;
	else
		return x * (x + 1) / 2;
}

std::ostream& bold(std::ostream& stream) {
    return stream << "\e[1m";
}

std::ostream& underline(std::ostream& stream) {
    return stream << "\e[4m";
}

std::ostream& normal(std::ostream& stream) {
    return stream << "\e[0m";
}

std::size_t pickRand(std::size_t from, std::size_t to) {
	return from + (std::rand() % (to - from));
}

template int triangular_number(int);
template std::size_t triangular_number(std::size_t);