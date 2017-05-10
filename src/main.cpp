#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
#include <getopt.h>
#include <cstdlib>

#include "Image.h"
#include "Drawer.h"
#include "optimization.h"
#include "cxxopts.h"


int main(int argc, char *argv[]) {

	std::string filename, fullname;
	int rows = 15, cols = 15, block_width = 5, wall_width = 1,resolution = 50; 
	bool obstacles = false, spaces = false, text = false;

	try {
        cxxopts::Options options(argv[0], "./maze_generator -f \"out\" -b 15 -w 3 -c 20 -r 20");

        options.add_options()
	        ("help", "Print help")
	        ("f,filename", "output filename", cxxopts::value<std::string>(filename)->default_value("out"))
	        ("x,rows", "rows in a maze", cxxopts::value<int>(rows)->default_value("15"))
	        ("y,cols", "colomns in a maze", cxxopts::value<int>(cols)->default_value("15"))
	        ("b,block_width", "pixel width of a block in a maze", cxxopts::value<int>(block_width)->default_value("15"))
	        ("w,wall_width", "pixel width of a wall in a maze", cxxopts::value<int>(wall_width)->default_value("3"))
	        ("r,resolution", "resolution in meteres per 1000 pixels", cxxopts::value<int>(resolution)->default_value("50"))
	        ("o,obstacles", "generate additional obstacles in a maze", cxxopts::value<bool>(obstacles))
	        ("s,spaces", "generate additional spaces in a maze", cxxopts::value<bool>(spaces))
	        ("t,text", "generate pgm file in text mode", cxxopts::value<bool>(text));


        options.parse(argc, argv);

        if (options.count("help")) {
            std::cout << options.help({""}) << std::endl;
            exit(0);
        }

        if (rows < 0 || cols < 0 || wall_width < 0 || block_width < 0 || resolution < 0) {
        	throw cxxopts::OptionException("Can't understand negative values");
        }

    } catch (const cxxopts::OptionException& e) {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
    fullname = filename + std::string(".pgm");
	freopen(fullname.c_str(), "wt", stdout);

	uint8_t color_black = 0;

	int pixels_width = cols * (block_width + wall_width) + wall_width;
	int pixels_height = rows * (block_width + wall_width) + wall_width;

	Image image(pixels_height, pixels_width, color_black);
	Drawer drawer(image);

	Maze maze(rows, cols);
	maze.generate();
	drawer.draw_maze(maze, block_width, wall_width);
	
	if (spaces)
		drawer.generate_random_objects(rows * cols / (rows + cols), block_width, 3 * block_width, Drawer::color_white, Drawer::color_black);

	if (obstacles)
		drawer.generate_random_objects(rows * cols / (rows + cols), block_width / 2, 2 * block_width, Drawer::color_black, Drawer::color_white);


	writeWord(text ? "P2\n" : "P5\n");
	writeInt(pixels_width, ' ');
	writeInt(pixels_height, '\n');
	writeInt(255, '\n');

	if (text)
		drawer.write_image();
	else
		drawer.write_image_bin();

	std::ofstream ofs(filename + std::string(".yaml"));
	char* full_path = realpath(fullname.c_str(), NULL);

	ofs << "image: " << full_path << '\n';
	ofs << "resolution: " << static_cast<float>(resolution)/1000 <<"\n";
	ofs << "origin: [-" << wall_width + block_width/2 << ", -" << wall_width + block_width/2 << ", 0.000000]\n";
	ofs << "negate: 0\noccupied_thresh: 0.65\nfree_thresh: 0.196";

	free(full_path);

	return 0;
}
