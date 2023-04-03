# Part 2 - Initial Design considerations and initial code

The hardest part of the design is going to be how to make the system work with both the drawing and collisions. For ease we could use a simple 2D style grid to store everything in, but I want to later expand to a 3D view camera so I'm going to discount this approach.

My main design is going to fix each snake segment to a unit cube, I can then make the arena a fixed size with the origin at 0,0 which is the center of the arena and the extents going in a +/- width / height.

To start with I will make them both 50 "units" each side, hitting any of the walls will cause a "game over" initially but I may change this later to teleport to the other side.

Will use as much build in NGL stuff to start with for ease, so nglColourShader for all rendering, and ngl::ortho and ngl::lookAt for the initial cameras. Rendering will be with VAOPrimitives using only cubes. 

## Initial steps

1. create a simple arena and snake class
2. setup camera and projection
3. Game loop to allow the snake to move
4. collisions with walls of arena

## Things to think about

1. how do I share the camera / projection with all the classes?
2. who's responsibility is managing things? (main.cpp or Arena?)


## Steps so far

1. copy SDLNGL CMakeLists.txt to project folder and edit to change name and remove some of the files
2. copy SDLNGL/src/main.cpp into src and edit to make simple loop
3. created empty class files for Arena, Snake and Food and added them to the CMakeLists.txt
4. uploaded to Github.

Will now start on [Part3.md](Part3.md)
