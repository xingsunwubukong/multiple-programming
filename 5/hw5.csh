#!/bin/csh

#number of size:
foreach size (1000 5000 10000 50000 100000 200000 500000 1000000 2000000 3000000 4000000 5000000 6000000 7000000 8388608 )

    g++ -DSIZE=$size hw5.cpp -o hw5 -lm -fopenmp
    ./hw5
end
