#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "maze_generator_utils.h"
#include "Image.h"
#include "Maze.h"

class Drawer {
public:
    Drawer(Image &I): picture(I) {};

    void draw_horizontal_line(std::size_t l, std::size_t i, std::size_t j, uint8_t val);
    void draw_vertical_line(std::size_t l, std::size_t i, std::size_t j, uint8_t val);

    void draw_circle(int x1, int y1, int R, uint8_t color, bool fill = true);
    void draw_rectangle(int x1, int y1, int x2, int y2, uint8_t color, bool fill = true);
    void draw_rectangle_with_border(int x1, int y1, int x2, int y2, uint8_t color);
    void generate_random_objects(std::size_t n, std::size_t min_size, std::size_t max_size, uint8_t color, uint8_t incolor, std::size_t reduce = 1);

    void draw_maze(Maze &maze, std::size_t cell_size, std::size_t wall_size);

    void write_image() const;
    void write_image_bin() const;


    const static uint8_t color_white = 254;
    const static uint8_t color_grey = 205;
    const static uint8_t color_black = 0;

private:
    Image &picture;
};

