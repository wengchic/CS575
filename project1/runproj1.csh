#!/bin/csh

#number of threads:
foreach threads ( 1 2 4 8 16 32)
	foreach numTrials ( 1 10 100 1000 10000 100000 1000000)

		g++ -DNUMT=$threads -DNUMTRIALS=$numTrials project1.cpp -o project1 -lm -fopenmp
		./project1
	end
end