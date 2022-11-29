#include <iostream>
#include <omp.h>
#include <math.h>
#include <stdlib.h>

void printState();
void computeWeather();
float SQR(float);
float Ranf(unsigned int*, float, float);
int Randf(unsigned int*, int, int);
unsigned int seed = 0;  // a thread-private variable

// The "state" of the system consists of the following global variables.
int	NowYear;			// 2017 - 2022
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;			// temperature this month
float	NowHeight;		// grain height in inches
int		NowNumDeer;		// number of deer in the current population
int   	Deer_parasite; 

// Initialize temperature and precipitation constants. 
const float GRAIN_GROWS_PER_MONTH =		8.0;	// inches
const float ONE_DEER_EATS_PER_MONTH =	0.5;

const float AVG_PRECIP_PER_MONTH =		6.0;	// average (inches)
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =				2.0;	// plus or minus noise

const float AVG_TEMP =						50.0;	// average (deg_F)
const float AMP_TEMP =						20.0;	// plus or minus
const float RANDOM_TEMP =					10.0;	// plus or minus noise

const float MIDTEMP =						40.0;
const float MIDPRECIP =						10.0;


// Compute next number of graindeer, based on set of global variables,
// into local, temporary, variables. 
void GrainDeer()
{
	
	while( NowYear < 2028)
	{
		
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		int tempNumDeer = NowNumDeer;
		
		if(tempNumDeer > NowHeight)
		{
			tempNumDeer--;
		}
		else
		{
			tempNumDeer++;
		}

		if(tempNumDeer < 0)
		{
			tempNumDeer = 0;
		}
		// DoneComputing barrier:
		#pragma omp barrier
		NowNumDeer = tempNumDeer;
	
		// DoneAssigning barrier:
		#pragma omp barrier
		
		// DonePrinting barrier:
		#pragma omp barrier
		//. . .
	}

}

// Compute next grain growth, based on set of global variables,
// into local, temporary, variables. 
void Grain()
{
	
		while( NowYear < 2028 )
	{
		
		// compute a temporary next-value for this quantity
		// based on the current state of the simulation:
		float tempNowHeight = NowHeight;
		float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
		float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
		
		tempNowHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
 		tempNowHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
 		
 		if(tempNowHeight < 0.)
 		{
 			tempNowHeight = 0.;
 		}
	
		// DoneComputing barrier:
		#pragma omp barrier
		NowHeight = tempNowHeight;
			
		// DoneAssigning barrier:
		#pragma omp barrier
		//. . .
	
		// DonePrinting barrier:
		#pragma omp barrier
		//. . .
	}
	
}

void Watcher()
{
	
		while( NowYear < 2028 )
	{
	
		// DoneComputing barrier:
		#pragma omp barrier

		// DoneAssigning barrier:
		#pragma omp barrier
		
		// Print current set of global state variables. 
		printState();
		
		// Increment month count.
		NowMonth++;
		
		if(NowMonth == 12)
		{
			NowMonth = 0;
			NowYear++; 
		}
		
		// Compute new temperature and precipitation. 
		computeWeather();
		
		// DonePrinting barrier:
		#pragma omp barrier
		//. . .
	}
	
}

// Aliens visit Grainville and control the growth of grain or the graindeer population. 
void deer_eating_parasite()
{
	
	while( NowYear < 2028 )
	{
		
		int NextNumWolves = Deer_parasite;
		if (Deer_parasite > NowNumDeer)
			NextNumWolves--;
		if (Deer_parasite < NowNumDeer)
			NextNumWolves++;
		if (NextNumWolves < 0)
			NextNumWolves = 0;
#pragma omp barrier
		Deer_parasite = NextNumWolves;
#pragma omp barrier
#pragma omp barrier
	
	}
}

// Calculate temperature and precipitation.
void computeWeather()
{
	
	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );

	NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );
	
	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
	if( NowPrecip < 0. )
	{
		NowPrecip = 0.;
	}
}

// Print current set of global state variables. 
void printState()
{
	
	std::cout << NowYear << "," << NowMonth << "," << (5./9.)*(NowTemp-32) << "," << NowPrecip*2.54 << "," << NowNumDeer << "," << NowHeight*2.54 << "," << Deer_parasite << "\n";
	
	/*
	std::cout << "Year #: " << NowYear << "\n";
	std::cout << "Month #: " << NowMonth << "\n";
	
	if(Deer_parasite == 1)
	{
		std::cout << "***Deer_parasite***" << "\n";
	}
	
	std::cout << "Temperature (F): " << NowTemp << "\n";
	std::cout << "Temperature (C): " << (5./9.)*(NowTemp-32) << "\n";
	std::cout << "Precipitation (in): " << NowPrecip << "\n";
	std::cout << "Precipitation (cm): " << NowPrecip*2.54 << "\n";
	std::cout << "Number of graindeer: " << NowNumDeer << "\n";
	std::cout << "Height of grain (in): " << NowHeight << "\n";
	std::cout << "Height of grain (cm): " << NowHeight*2.54 << "\n";
	*/
}

// Utility function.
float SQR(float x)
{
	
	return x*x;

}

// Random utility function. 
float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}

int main()
{
	#ifndef _OPENMP
			fprintf( stderr, "No OpenMP support!\n" );
	#endif
	// Starting date and time:
	NowMonth =    0;
	NowYear  = 2022;

	// Starting state (feel free to change):
	NowNumDeer = 10;
	NowHeight =  1.;
	Deer_parasite = 4;
	// Know how many threads already, start them with parallel sections
	omp_set_num_threads( 4 );	// same as # of sections
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			GrainDeer( );
		}
	
		#pragma omp section
		{
			Grain( );
		}
	
		#pragma omp section
		{
			Watcher( );
		}
	
		#pragma omp section
		{
			deer_eating_parasite( );	
		}
	}	// implied barrier -- all functions must return in order
		// to allow any of them to get past here
}