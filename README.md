# Maze generator for ROS

There is a small program to generate mazes.

Prerequisites
-------------

* GNU Make 4.0+
* gcc 5.0+
* cpp std 14

Installation
------------
```
make # or make std to clear your terminal
```

Example
-------
```
 ./maze_generator -x 40 -y 60 -b 20 -w 5 --spaces
```

Output
------

The generator creates a ".pgm" and ".yaml" files to load in your map server

![Grid map example](https://pp.userapi.com/c636427/v636427848/63566/pqz7axVrZ2w.jpg)

You also can change any parameters:

![Map with spaces and obstacles](https://pp.userapi.com/c636427/v636427848/6356d/RCJ3Xdj9fq0.jpg)

.yaml example:

      image: /home/kostya55516/Documents/maze_generator/out.pgm
      resolution: 0.05
      origin: [-20, -20, 0.000000]
      negate: 0
      occupied_thresh: 0.65
      free_thresh: 0.196
      
Warning
-------

If you need to create largest maze ever you will have to expand your stack.

Maximum maze size is approximately 550 x 550 blocks
