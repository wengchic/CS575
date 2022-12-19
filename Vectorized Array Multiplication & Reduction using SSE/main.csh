#!/bin/csh

echo "Result:"
echo "ARRAYSIZE, Speedup, SIMD Performance, Non-SIMD Performance"

foreach ARRAYSIZE (1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608)

	g++ -DARRAYSIZE=$ARRAYSIZE main.cpp -o main -lm -fopenmp
	./main

end