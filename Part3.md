# Part3 Getting the snake class to build and draw

In this part I'm going to get the basic camera, snake cube and other elements setup so I can use them.

## Design Decisions 

- Our world will use normal 3d, so we are looking from above down the -ve Y axis. So Snake pos is a Vec3 with y clamped to 0.0f to start with.
- All segments are unit cubes. I will set this up in the initial main.cpp, as with the camera which will be passed through to the Snake (not sure if I will inject this or construct with it).

- Both Camera and Projection will be ngl::Mat4 (most likely a reference)

1. create snake ctor and basic attributes. A snake will have a position, direction and a list of segments. 
2. started making the snake but it didn't work well.
3. Created a branch to develop proper snake (snake-refactor)
4. Now working with basic self collision and timings


Now need to check with collisions with arena and decide how to link things together. Will cover this in [Part4.md](Part4.md)





