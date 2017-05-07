#pragma once
#include "random"

class Maze {
public:
	Maze(std::size_t r, std::size_t c);
	~Maze();

	const int8_t* operator[](std::size_t y) const {
		return parent[y];
	}

	std::size_t get_cols() const {
		return cols;
	}

	std::size_t get_rows() const {
		return rows;
	}

	int8_t get_x_move(std::size_t i) const {
		return x_move[i];
	}

	int8_t get_y_move(std::size_t i) const {
		return y_move[i];
	}

	void generate(int dir = 5, int x = 0, int y = 0);
private:
	int8_t **parent;
	std::size_t cols, rows;
	int8_t x_move[4] = {1, 0, -1, 0};
	int8_t y_move[4] = {0, -1, 0, 1};
	std::random_device rd;
	std::mt19937 mt;
};