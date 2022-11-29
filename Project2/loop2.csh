#!/bin/bash

for t in 1 2 4 8 12 16 32
do
	for n in 10 100 200 300 400 500 600 700
	do
		g++ -O3 Project_3new.cpp -DNUMT=$t -DNUMNODES=$n -o functional_decomposition -lm -fopenmp
		./functional_decomposition
	done
done