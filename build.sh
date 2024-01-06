#!/bin/bash

clang -g -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -Lraylib5 -lraylib main.c snake.c -o main


