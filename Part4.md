# Part4 Getting the arena to work.

My thinking is that I can move all of the game logic into the arena and this will make it easier for the main.cpp program to control the game etc.

This also means that I can later change to another API (Qt for example) if I need to as the main logic will be in the arena. 

First the Arena can be constructed with the width and height, I guess the camera and other elements can be created there as well and all other classes can pass in these. 

