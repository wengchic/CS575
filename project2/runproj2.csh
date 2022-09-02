#!/bin/csh

#number of threads:
foreach threads ( 1 2 4 8)
	foreach numNodes ( 4 8 16 32 64 128 256 512 )

		g++ -DNUMT=$threads -DNUMNODES=$numNodes project2.cpp -o project2 -lm -fopenmp
		./project2
	end
end