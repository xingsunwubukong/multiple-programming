#!/bin/csh

#number of threads:
foreach threads ( 1 2 4 8 16 32)
	foreach numTrials ( 1 10 100 1000 10000 100000 1000000)

		g++ -DNUMT=$threads -DNUMTRIALS=$numTrials sunhw2.cpp -o sunhw2 -lm -fopenmp
		./sunhw2
	end
end