#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

#define N	      0.70

#ifndef NUMNODES
#define NUMNODES  1000	
#endif

#ifndef NUMT
#define NUMT	  1	
#endif

#ifndef NUMTRIES
#define NUMTRIES  10
#endif

float Height(int, int);



int main(int argc, char* argv[])
{
	double volume;	
	omp_set_num_threads(NUMT);

	// the area of a single full-sized tile:
	// (not all tiles are full-sized, though)


	float fullTileArea = (((XMAX - XMIN) / (float)(NUMNODES - 1)) *
		                 ((YMAX - YMIN) / (float)(NUMNODES - 1)));
 
	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:

	double maxPerformance = 0.;

	for (int tries = 0; tries < NUMTRIES; tries++)
	{
        volume = 0.;

        double time0 = omp_get_wtime();

        #pragma omp parallel for collapse(2) default(none) shared(fullTileArea) reduction(+:volume)

		for (int iv = 0; iv < NUMNODES; iv++)
		{
			for (int iu = 0; iu < NUMNODES; iu++)
			{
				
			  float s = 1.;


			  if      ((iu == 0 || iv == 0 || iu == NUMNODES - 1 || iv == NUMNODES - 1))
				 
				  s = 1/2;

			  else if ((iu == 0 && iv == 0) || (iu == 0 && iv == NUMNODES - 1) || (iu == NUMNODES - 1 && iv == 0) || (iu == NUMNODES-1 && iv == NUMNODES - 1))

				  s = 1/4;

			  float h =  Height(iu, iv);

			  volume = volume + 2 * h * s * fullTileArea;
			}
		}
		double time1 = omp_get_wtime();
		float megaheightPerSecond = (float)NUMNODES * (float)NUMNODES / (time1 - time0) / 1000000.;
		if (megaheightPerSecond > maxPerformance)
		    maxPerformance = megaheightPerSecond;
    }


    fprintf(stderr, "%2d Numt ; %8d NUMNODES ; Megaheight/sec = %6.2lf ; Volume= %f\n",NUMT, NUMNODES, maxPerformance, volume );


	return 0;
}

float
Height(int iu, int iv)	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1. + 2. * (float)iu / (float)(NUMNODES - 1);	// -1. to +1.
	float y = -1. + 2. * (float)iv / (float)(NUMNODES - 1);	// -1. to +1.

	float xn = pow(fabs(x), (double)N);
	float yn = pow(fabs(y), (double)N);
	float r = 1. - xn - yn;
	if (r <= 0.)
		return 0.;
	float height = pow(r, 1. / (float)N);
	return height;
}