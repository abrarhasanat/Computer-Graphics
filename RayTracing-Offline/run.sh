!/bin/bash
clear
rm test.bmp
rm demo
g++ 1805023_Main.cpp -o demo -lglut -lGLU -lGL 
./demo
