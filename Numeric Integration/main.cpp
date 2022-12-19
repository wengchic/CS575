#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

// setting the number of threads:
#ifndef NUMT
#define NUMT		1	
#endif

// setting the number of subdivisions:
#ifndef NUMNODES
#define NUMNODES	1000		
#endif

// how many tries to discover the maximum performance:
#ifndef NUMTRIES
#define NUMTRIES	100
#endif

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

#define N	0.70

float Height( int, int );	// function prototype

int main( int argc, char *argv[ ] )
{
	#ifndef _OPENMP
		fprintf( stderr, "No OpenMP support!\n" );
		return 1;
	#endif

	omp_set_num_threads(NUMT);	// set the number of threads to use in the for-loop:`


	// the area of a single full-sized tile:
	// (not all tiles are full-sized, though)

	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES - 1) ) * ( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );
	double maxPerformance = 0.;	// must be declared outside the NUMTRIES loop
	double sumVolume = 0.;

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:
	for(int t = 0; t < NUMTRIES; t++){

		double time0 = omp_get_wtime();
		double volume = 0.;

		#pragma omp parallel for default(none) shared(fullTileArea, sumVolume) reduction(+:volume)
		for( int i = 0; i < NUMNODES*NUMNODES; i++ )
		{
			int iu = i % NUMNODES;
			int iv = i / NUMNODES;
			float z = Height( iu, iv );
			double tmpTileArea = fullTileArea;

			if ((iu == NUMNODES - 1 || iu == 0) && (iv == NUMNODES - 1 || iv == 0)) {
				tmpTileArea -= fullTileArea * 0.75;
				

			}else if ((iu == NUMNODES - 1 || iu == 0) || (iv == NUMNODES - 1 || iv == 0)) {
				tmpTileArea -= fullTileArea * 0.5;
	

			}
			volume += 2 * tmpTileArea * z;
		
	
		}
		double time1 = omp_get_wtime();
		double megaHeightsPerSecond = (double)NUMNODES * NUMNODES / (time1 - time0) / 1000000.;

		if (megaHeightsPerSecond > maxPerformance) {
            maxPerformance = megaHeightsPerSecond;

	}
		sumVolume += volume;

	}
	double avgVolume = sumVolume / NUMTRIES;
	printf("%2d threads ; %2d NUMNODES ; maxPerformance = %6.2lf ; avgVolume = %6.2lf\n", NUMT, NUMNODES, maxPerformance, avgVolume);

    return 0;

}




float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float r = 1. - xn - yn;
	if( r <= 0. )
	        return 0.;
	float height = pow( r, 1./(float)N );
	return height;
}