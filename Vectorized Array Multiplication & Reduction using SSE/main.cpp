#include <xmmintrin.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef ARRAYSIZE 
#define ARRAYSIZE		1024 
#endif

#ifndef NUMTRIES 
#define NUMTRIES		20 
#endif

#define SSE_WIDTH		4

float a[ARRAYSIZE];
float b[ARRAYSIZE];
float c[ARRAYSIZE];



void SimdMul( float *a, float *b,   float *c,   int len ){
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


void NonSimdMul( float *a, float *b, float *c, int len ){

	for (int i = 0; i < len; i++)
	{
		c[i] = a[i] * b[i]; 
	}	  

}

int main(int argc, char *argv[ ]){
	double maxPerformanceSIMD = 0.;
	double maxPerformanceNoSIMD = 0.;
	double Speedup = 0.;


    for (int t = 0; t < NUMTRIES; t++) {
       

        double time0SIMD = omp_get_wtime();
        SimdMul(a, b, c, ARRAYSIZE);
        double time1SIMD = omp_get_wtime();


        double megaMulsPerSecSIMD = (double)ARRAYSIZE / (time1SIMD - time0SIMD) / 1000000.;
        if (megaMulsPerSecSIMD > maxPerformanceSIMD) {
            maxPerformanceSIMD = megaMulsPerSecSIMD;
        }

        double time0NoSIMD = omp_get_wtime();
        NonSimdMul(a, b, c, ARRAYSIZE);
        double time1NoSIMD = omp_get_wtime();

        
        double megaMulsPerSecNoSIMD = (double)ARRAYSIZE / (time1NoSIMD - time0NoSIMD) / 1000000.;
        if (megaMulsPerSecNoSIMD > maxPerformanceNoSIMD) {
            maxPerformanceNoSIMD = megaMulsPerSecNoSIMD;
        }

    }

    Speedup = maxPerformanceSIMD / maxPerformanceNoSIMD;

    printf("%d, %.3lf, %.3lf, %.3lf\n", ARRAYSIZE, Speedup, maxPerformanceSIMD, maxPerformanceNoSIMD);

    return 0;

}



