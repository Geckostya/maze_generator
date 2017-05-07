#include <random>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#include "Drawer.h"
#include "optimization.h"

#define PLACE() __FILE__, __FUNCTION__, __LINE__

void Drawer::draw_horizontal_line(std::size_t l, std::size_t i, std::size_t j, uint8_t val) {
    for (std::size_t k = i; k <= j; k++) {
        picture[k][l] = val;
    }
}

void Drawer::draw_vertical_line(std::size_t l, std::size_t i, std::size_t j, uint8_t val) {
    for (std::size_t k = i; k <= j; k++) {
        picture[l][k] = val;
    }
}

void Drawer::writeImage() const {
    for (std::size_t y = 0; y < picture.get_rows(); y++) 
        for (std::size_t x = 0; x < picture.get_cols(); x++)
            writeInt(static_cast<int>(picture[y][x]), ' ');
}

void Drawer::draw_circle(int x1, int y1, int R, uint8_t color, bool fill) {
    int x = 0;
    int y = R;
    int delta = 1 - 2 * R;
    int error = 0;
    while (y >= 0) {
        std::size_t up    = static_cast<std::size_t>(clamp(y1 - y, 0, static_cast<int>(picture.get_cols()) - 1));
        std::size_t down  = static_cast<std::size_t>(clamp(y1 + y, 0, static_cast<int>(picture.get_cols()) - 1));
        std::size_t left  = static_cast<std::size_t>(clamp(x1 - x, 0, static_cast<int>(picture.get_rows()) - 1));
        std::size_t right = static_cast<std::size_t>(clamp(x1 + x, 0, static_cast<int>(picture.get_rows()) - 1));  

        if (fill) {
            this->draw_vertical_line(right, up, down, color);
            this->draw_vertical_line(left, up, down, color);
        } else {
            picture[right][up]   = color;
            picture[right][down] = color;
            picture[left][up]    = color;
            picture[left][down]  = color;
        }

        error = 2 * (delta + y) - 1;
        if (delta < 0 && error <= 0) {
            delta += 2 * (++x) + 1;
            continue;
        }
        error = 2 * (delta - x) - 1;
        if (delta > 0 && error > 0) {
            delta += 1 - 2 * (--y);
            continue;
        }
        x++;
        delta += 2 * (x - y);
        y--;
    }
}

void Drawer::draw_rectangle(int x1, int y1, int x2, int y2, uint8_t color, bool fill) {
    if (x2 < x1)
        std::swap(x1, x2);
    if (y2 < y1)
        std::swap(y1, y2);
    x1 = clamp(x1, 0, static_cast<int>(picture.get_cols()) - 1);
    x2 = clamp(x2, 0, static_cast<int>(picture.get_cols()) - 1);
    y1 = clamp(y1, 0, static_cast<int>(picture.get_rows()) - 1);
    y2 = clamp(y2, 0, static_cast<int>(picture.get_rows()) - 1);
    
    if (fill)
        for (int i = x1; i <= x2; i++)
            for (int j = y1; j <= y2; j++)
                    picture[j][i] = color; 
    else {
        for (int i = x1; i <= x2; i++) {
            picture[y1][i] = color;
            picture[y2][i] = color; 
        }
        for (int i = y1 + 1; i < y2; i++) {
            picture[i][x1] = color;
            picture[i][x2] = color;
        }
    }
}

void Drawer::draw_rectangle_with_border(int x1, int y1, int x2, int y2, uint8_t color) {
    this->draw_rectangle(x1, y1, x2, y2, color, false);
    this->draw_rectangle(x1 - 1, y1-1, x2 - 1, y2 - 1, color, true);
}

void Drawer::generate_random_objects(std::size_t n, std::size_t min_size, std::size_t max_size, uint8_t color, uint8_t incolor, std::size_t reduce) {
    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<int>  place_r(0, picture.get_rows() - 1),
                                        place_c(0, picture.get_cols() - 1),
                                        size_r(min_size, std::min(max_size, picture.get_rows() - 1)),
                                        size_c(min_size, std::min(max_size, picture.get_cols() - 1));

    int attempts = 10;
    for(std::size_t i = 0; i < n; i++) {
        for (int j = 0; j < attempts; j++) {
            int x1 = place_r(mt);
            int y1 = place_c(mt);
            if (picture[x1][y1] != incolor && j < attempts - 1)
                continue;

            if (place_r(mt) % 2) {
                draw_rectangle(x1, y1, x1 + size_r(mt) / reduce, y1 + size_c(mt) / reduce, color);
            } else {
                int radius = 1;
                if (picture.get_cols() < picture.get_rows()) {
                    radius = size_c(mt) / 2 / reduce; 
                } else {
                    radius = size_r(mt) / 2 / reduce; 
                }
                draw_circle(x1, y1, radius, color);
            }
            break;
        }
    }
}

void Drawer::draw_maze(Maze &maze, std::size_t cell_size, std::size_t wall_size) {
    std::size_t cols = maze.get_cols();
    std::size_t rows = maze.get_rows();
    for (std::size_t y = 0; y < rows; y++)
        for (std::size_t x = 0; x < cols; x++) {
            int8_t dir = maze[y][x];
            
            int start_x = x * cell_size + (x + 1) * wall_size;
            int start_y = y * cell_size + (y + 1) * wall_size;
            int end_x = x * cell_size + (x + 1) * wall_size + cell_size - 1;
            int end_y = y * cell_size + (y + 1) * wall_size + cell_size - 1;

            draw_rectangle(start_x, start_y, end_x, end_y, color_white, true);

            if (dir == 5)
                continue;
            if (maze.get_x_move(dir) == 1) 
                draw_rectangle(end_x + 1, start_y, end_x + wall_size, end_y, color_white, true);
            else if (maze.get_x_move(dir) == -1)
                draw_rectangle(start_x - wall_size, start_y, start_x - 1, end_y, color_white, true);
            else if (maze.get_y_move(dir) == 1)
                draw_rectangle(start_x, end_y + 1, end_x, end_y + wall_size, color_white, true);
            else if (maze.get_y_move(dir) == -1)
                draw_rectangle(start_x, start_y - wall_size, end_x, end_y - 1, color_white, true);
        }
}