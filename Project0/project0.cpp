#include <omp.h>
#include <stdio.h>
#include <math.h>

#define NUMT	         1	// number of threads to use
#define SIZE       	70000// array size -- you get to decide
#define NUMTRIES        3500	// how many times to run the timing -- you get to decide

float A[SIZE];
float B[SIZE];
float C[SIZE];

int
main( )
{
#ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
#endif

	// inialize the arrays:
	for( int i = 0; i < SIZE; i++ )
	{
		A[i] = 1.;
		B[i] = 2.;
	}

        omp_set_num_threads( NUMT );
        fprintf( stderr, "Using %d threads\n", NUMT );
		
		double maxMegaMults = 0.;
		double sumMegaMults = 0.;

        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                #pragma omp parallel for
                for( int i = 0; i < SIZE; i++ )
                {
                        C[i] = A[i] * B[i];
                }

                double time1 = omp_get_wtime( );
                double megaMults = (double)SIZE/(time1-time0)/1000000.;
				sumMegaMults += megaMults;
                if( megaMults > maxMegaMults )
                        maxMegaMults = megaMults;
        }

        printf( "Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults );
		printf( "Average Performance = %8.2lf MegaMults/Sec\n", sumMegaMults/double(NUMTRIES));

	// note: %lf stands for "long float", which is how printf prints a "double"
	//        %d stands for "decimal integer", not "double"

        return 0;
}