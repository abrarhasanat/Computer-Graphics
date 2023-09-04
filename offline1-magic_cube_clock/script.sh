#!/bin/bash
rm demo
#get the name from parameter and store it in a variable
name=$1
g++ $name  -o demo -lglut -lGLU -lGL
./demo
