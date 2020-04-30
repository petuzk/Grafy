#pragma once

/*
 * Cell jest kontenerem dla jednego obiektu
 */

template <typename T>
class Cell {
	T item;

public:
	Cell(T obj);
	const T& getItem() const;
	void setItem(T obj);
};