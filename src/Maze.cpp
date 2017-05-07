#include "Maze.h"
#include "algorithm"

Maze::Maze(std::size_t r, std::size_t c) : cols(c), rows(r) {
	mt.seed(rd());

	int8_t *tmp = new int8_t [r * c];
    std::fill(tmp, tmp + r * c, -1);
    parent = new int8_t *[r];
    
    for (std::size_t i = 0; i < r; i++)
        parent[i] = tmp + i * c;	
}

Maze::~Maze() {
	delete[] parent[0];
    delete[] parent;
}

void Maze::generate(int dir, int x, int y) {
	if (x < 0 || x >= static_cast<int>(cols) || y < 0 || y >= static_cast<int>(rows) || parent[y][x] != -1)
		return;

	int8_t direction[4] = {0, 1, 2, 3};
	std::shuffle(direction, direction + 4, mt);

	parent[y][x] = dir; 

	for (uint8_t d : direction) {
		generate((d + 2) % 4, x + static_cast<int>(x_move[d]), y + static_cast<int>(y_move[d]));
	}
}