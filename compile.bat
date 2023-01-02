@echo off

g++ demo.cpp -o demo -IC:\SDL32\include\SDL2 -LC:\SDL32\lib  -lmingw32 -lSDL2main -lSDL2 -D WIN

