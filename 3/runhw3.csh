#!/bin/csh

# number of threads:
foreach t (1 2 4 8 16)
foreach s (10 20 40 80 160 320 640 1280)

                g++ -DNUMNODES=$s -DNUMT=$t hw3.cpp -o hw3 -lm -fopenmp
                ./hw3
        end
end