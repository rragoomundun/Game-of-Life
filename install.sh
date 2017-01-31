#!/bin/bash

cd src/ && gcc -Wall -Wextra *.c -o game -lSDL2 && cd .. && mkdir out && mv src/game out/
