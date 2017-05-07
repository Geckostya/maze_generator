#include "Image.h"
#include "optimization.h"

#define PLACE() __FILE__, __FUNCTION__, __LINE__

uint8_t** Image::new_data(std::size_t r, std::size_t c, uint8_t x) {
    uint8_t *tmp = new uint8_t [r * c];
    std::fill(tmp, tmp + r * c, x);
    uint8_t **data = new uint8_t *[r];
    for (std::size_t i = 0; i < r; i++)
        data[i] = tmp + i * c;
    return data;
}

void Image::del_data() {
    delete[] _data[0];
    delete[] _data;
}

Image::Image(std::size_t r, std::size_t c, uint8_t x) {
    _data = new_data(r, c, x);
    _rows = r;
    _cols = c;
}

Image::~Image() {
    del_data();
}

Image::Image(const Image& m) {
    _rows = m._rows;
    _cols = m._cols;
    _data = new_data(_rows, _cols, 0);
    for (std::size_t i = 0; i < _rows; i++)
    for (std::size_t j = 0; j < _cols; j++)
        _data[i][j] = m._data[i][j];
}

void Image::set(std::size_t i, std::size_t j, uint8_t val) {
    if (i >= _rows || j >= _cols)
        throw ImageException("Out of range", PLACE());
    _data[i][j] = val;
}

uint8_t Image::get(std::size_t i, std::size_t j) const {
    if (i >= _rows || j >= _cols)
        throw ImageException("Out of range", PLACE());
    return _data[i][j];
}

ImageException::ImageException(const char* msg) {
    message = msg;
}

ImageException::ImageException(const char* msg, const char* file, const char* func, int line) {
    message = msg;
    error_dest = std::string("Error in file ") + file 
                 + std::string(", function ") +  func 
                 + std::string(", in line ") + std::to_string(line); 
}