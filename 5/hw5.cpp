#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <xmmintrin.h>
#define SSE_WIDTH		4

#ifndef NUM
#define NUM  30
#endif

#ifndef SIZE
#define SIZE   1000	
#endif

float ARRAYONE[SIZE];
float ARRAYTWO[SIZE];
float ARRAYTHREE[SIZE];


void
SimdMul( float *a, float *b,   float *c,   int len )
{
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;
	register float *pc = c;
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		_mm_storeu_ps( pc,  _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
		pc += SSE_WIDTH;
	}

	for( int i = limit; i < len; i++ )
	{
		c[i] = a[i] * b[i];
	}
}

void
NonSimdMul(float* a, float* b, float* c, int len)
{
	for (int i = 0; i < len; i++)
	{
		c[0] += a[i] * b[i];
	}

}


float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;

	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; i++ )
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}
int main(int argc, char* argv[])
{
	double maxMegaMultsPerSecond = 0.;
	double nonmaxMegaMultsPerSecond = 0.;

	for (int a = 0; a < SIZE; a++) {
		ARRAYONE[a] = 1.5;
		ARRAYTWO[a] = 2.5;

	}

	for (int t = 0; t < NUM; t++)
	{
	
		double time0 = omp_get_wtime();
		SimdMul(ARRAYONE, ARRAYTWO, ARRAYTHREE, SIZE);
		double time1 = omp_get_wtime();
		double nontime0 = omp_get_wtime();
		NonSimdMul(ARRAYONE, ARRAYTWO, ARRAYTHREE, SIZE);
		double nontime1 = omp_get_wtime();
		

		double megaMultsPerSecond = (double)SIZE / (time1 - time0) / 1000000.;
		if (megaMultsPerSecond > maxMegaMultsPerSecond)
			maxMegaMultsPerSecond = megaMultsPerSecond;

		double nonmegaMultsPerSecond = (double)SIZE / (nontime1 - nontime0) / 1000000.;
		if (nonmegaMultsPerSecond > nonmaxMegaMultsPerSecond)
			nonmaxMegaMultsPerSecond = nonmegaMultsPerSecond;

	}
	printf("    Size: %2d   Speed:%lf   SIMD: %lf   NONSIMDZ: %lf\n", SIZE, maxMegaMultsPerSecond/nonmaxMegaMultsPerSecond, maxMegaMultsPerSecond, nonmaxMegaMultsPerSecond);
	return 0;
} 
