#pragma once

#include <ostream>
#include <cstdlib>

/*
 * Zwraca liczbę trójkątną
 * pl.wikipedia.org/wiki/Liczba_trójkątna
 * en.wikipedia.org/wiki/Triangular_number
 */

template <typename T>
T triangular_number(T x);

/*
 * Ustawienia czcionki
 */

std::ostream& bold(std::ostream& stream);
std::ostream& underline(std::ostream& stream);
std::ostream& normal(std::ostream& stream);

/*
 * Zwraca losową liczbę całkowitą z przedziału [from, to)
 */

std::size_t pickRand(std::size_t from, std::size_t to);