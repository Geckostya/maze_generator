#pragma once

#include <string>
#include "maze_generator_utils.h"

class Image {
public:
    Image(std::size_t r, std::size_t c, uint8_t x = 205);
    ~Image();
    Image(const Image& m);

    std::size_t get_rows() const {
        return _rows; 
    }
    
    std::size_t get_cols() const {
        return _cols; 
    }

    void set(std::size_t i, std::size_t j, uint8_t val);
    uint8_t get(std::size_t i, std::size_t j) const;

    Image& operator=(const Image& m);
    uint8_t* operator[](std::size_t i) {
        return _data[i];
    }

    const uint8_t* operator[](std::size_t i) const {
        return _data[i];
    }

private:
    uint8_t** new_data(std::size_t r, std::size_t c, uint8_t x);
    void del_data();

    std::size_t _rows;
    std::size_t _cols;
    uint8_t **_data = NULL;
};

class ImageException {
public:
    std::string message, error_dest = "unknown";
    int line_number;
    ImageException(const char* msg);

    ImageException(const char* msg, const char* file, const char* func, int line);

    std::string what() const {
        return message;
    }

    std::string destination() const {
        return error_dest;
    } 
};
