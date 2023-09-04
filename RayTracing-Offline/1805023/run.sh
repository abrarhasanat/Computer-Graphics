!/bin/bash
clear
rm 1805023.bmp
rm 1805023_texture.bmp
rm demo
g++ 1805023_Main.cpp -o demo -lglut -lGLU -lGL 
./demo
