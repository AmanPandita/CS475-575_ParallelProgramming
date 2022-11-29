#include <omp.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

#ifndef NUMT
#define NUMT            2
#endif

#ifndef NUMNODES
#define NUMNODES     100
#endif
const float N = 2.5f;
const float R = 1.2f;


float Height( int, int );   // function prototype

int main( int argc, char *argv[] )
{
    #ifndef _OPENMP
    fprintf( stderr, "OpenMP is not available\n" );
    return 1;
    #endif

    omp_set_num_threads( NUMT );
    //int numProcessors = omp_get_num_procs( );
    //fprintf( stderr, "Have %d processors.\n", numProcessors );



    // the area of a single full-sized tile:
    // (not all tiles are full-sized, though)

    float FinalAreaTile = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
                ( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );
                
    
    
    
    double time0 =omp_get_wtime( );
    float Volume=0;
        #pragma omp parallel for collapse(2) default(none), shared(FinalAreaTile), reduction(+:Volume) 
        for( int iv = 0; iv < NUMNODES; iv++ )
        {
            
            for( int iu = 0; iu < NUMNODES; iu++ )
            {
                
                float z = Height( iu, iv );
                if((iu==0 && iv==0)||(iu==0 && iv==NUMNODES-1)||(iu==NUMNODES-1 && iv==NUMNODES-1)||(iu==NUMNODES-1 && iv==0)){
                Volume+=((FinalAreaTile/4)*z);
                }
                else if( iu==0 || iv==0 || iu==NUMNODES-1 || iv==NUMNODES-1){
                Volume+=((FinalAreaTile/2)*z);
                }
                else{
                Volume+=(FinalAreaTile*z);
                }       
        
        }

}
                Volume= 2*Volume;
                double time1=omp_get_wtime( );
                double megaHeightsPerSecond = (double)(NUMNODES*NUMNODES)/(time1-time0)/1000000.;
                


                fprintf(stderr, "%2d,%8d,%6.2lf,%f\n",NUMT, NUMNODES, megaHeightsPerSecond, Volume);

    
}


float Height( int iu, int iv )  
{
    float x1 = -1.  +  2.*(float)iu /(float)(NUMNODES-1);    // -1. to +1.
    float y1 = -1.  +  2.*(float)iv /(float)(NUMNODES-1);    // -1. to +1.

    float x1n = pow( fabs(x1), (double)N );
    float y1n = pow( fabs(y1), (double)N );
    float rn = pow( fabs(R), (double)N );
    float r = rn - x1n - y1n;
    if( r <= 0. )
            return 0.;
    float height = pow( r, 1./(double)N );
    return height;
}