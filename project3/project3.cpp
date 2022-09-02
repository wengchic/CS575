#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using std:: ofstream;
using std:: cout;
using std:: endl;
using std:: left;
using std:: setw;  

int	NowYear;		// 2021 - 2026
int	NowMonth;		// 0 - 11

float	NowPrecip;		// inches of rain per month
float	NowTemp;		// temperature this month
float	NowHeight;		// grain height in inches
int	NowNumDeer;		// number of deer in the current population
int NowPrintMonth;
int NowDeathDeer;

const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =		1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;

const float MIN_death =				1;
const float MAX_death =				2;

float Ranf(unsigned int, float, float);
int Ranf(unsigned int, int, int);

float Ran(float, float);
int Ran(int, int);


unsigned int seed = 0;


float Ran( float low, float high ){
        float r = (float) rand();               // 0 - RAND_MAX
        float t = r  /  (float) RAND_MAX;       // 0. - 1.

        return   low  +  t * ( high - low );
}

int Ran( int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = ceil( (float)ihigh );

        return (int) Ran(low,high);
}

float Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}



int Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

        return (int)(  Ranf(seedp, low,high) );
}

float x = Ranf( &seed, -1.f, 1.f );

float SQR( float x )
{
        return x*x;
}

void enviroment(){
	float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

	float temp = AVG_TEMP - AMP_TEMP * cos( ang );
	NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

	float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
	NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );

	if( NowPrecip < 0. ){
		NowPrecip = 0.;
	}
}


//section-----------------------

void Deer(){
	while( NowYear < 2027 ){
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
		int nextNumDeer = NowNumDeer;

		int carryingCapacity = (int)( NowHeight );

		if( nextNumDeer < carryingCapacity ){
	    	nextNumDeer++;

		}else if( nextNumDeer > carryingCapacity ){
	        nextNumDeer--;
	    }

	    if (NowDeathDeer >= 3){
	    	nextNumDeer--;

	    }


		if( nextNumDeer < 0 ){
	    	nextNumDeer = 0;
		}



		// DoneComputing barrier:
		#pragma omp barrier
		NowNumDeer = nextNumDeer;

		// DoneAssigning barrier:
		#pragma omp barrier
		

		// DonePrinting barrier:
		#pragma omp barrier
	
	}
}

void Grain(){
	while( NowYear < 2027 ){
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
	float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );

	float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );



	// DoneComputing barrier:
	#pragma omp barrier
	float nextHeight = NowHeight;
 	nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
 	nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;

 	if( nextHeight < 0. ){
 		nextHeight = 0.;

 	}

 	NowHeight = nextHeight;

	// DoneAssigning barrier:
	#pragma omp barrier
	

	// DonePrinting barrier:
	#pragma omp barrier
	
	}
}

void Watcher(){
	cout 	<< setw(10) << left << "Month"
			<< setw(10) << left << "Deer"
			<< setw(10) << left << "Height(Cm)"   
			<< setw(15) << left << "Temperature(C)" 
			<< setw(20) << left << "Precipitation(Cm)" 
			<< setw(10) << left << "DeathDeer" << endl;	

	ofstream project3output;
	project3output.open("project3output.csv");
	project3output << "Month,Deer,Height(Cm),Temperature(C),Precipitation(Cm),DeathDeer" << endl;

	while( NowYear < 2027 ){
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
	

	// DoneComputing barrier:
	#pragma omp barrier
	

	// DoneAssigning barrier:
	#pragma omp barrier
	cout 	<< setw(10) << left << NowPrintMonth+1
			<< setw(10) << left << NowNumDeer
			<< setw(10) << left << (NowHeight * 2.54)  
			<< setw(15) << left << (5./9.)*(NowTemp-32)
			<< setw(20) << left << (NowPrecip * 2.54) 
			<< setw(10) << left << NowDeathDeer << endl;	

	project3output 	<< NowPrintMonth+1 << "," 
					<< NowNumDeer << ","
					<< (NowHeight * 2.54) << ","  
					<< (5./9.)*(NowTemp-32) << "," 
					<< (NowPrecip * 2.54) << "," 
					<< NowDeathDeer << endl;



	if (NowMonth >= 11) {
    	NowMonth = 0;
        NowYear++;
    
    }else {
    	NowMonth++;
    
    }
    	NowPrintMonth++;	

    enviroment();


    //printf("%d Month; %d NowNumDeer; %lf NowHeight; %lf NowTemp; %lf NowPrecip; %d NowDeathDeer\n", 
    //	NowPrintMonth, NowNumDeer, (NowHeight * 2.54),(5./9.)*(NowTemp-32), (NowPrecip * 2.54), NowDeathDeer);



	// DonePrinting barrier:
	#pragma omp barrier
	
	}
}


//  dead of deer
void MyAgent(){
	while( NowYear < 2027 ){
	// compute a temporary next-value for this quantity
	// based on the current state of the simulation:
	int TempDeathDeer = NowDeathDeer; 

	if (((NowMonth >= 4) && (NowMonth <= 9)) && (NowNumDeer >= 5)) {
		TempDeathDeer += Ran( MIN_death, MAX_death );

	}else{
		TempDeathDeer --; 	
	}
	
	if (TempDeathDeer < 0){
		TempDeathDeer = 0;
	
	}

	// DoneComputing barrier:
	#pragma omp barrier
	NowDeathDeer = TempDeathDeer;

	// DoneAssigning barrier:
	#pragma omp barrier
	

	// DonePrinting barrier:
	#pragma omp barrier
	
	}
}

// main ------------------
int main(){

	// starting date and time:
	NowMonth =    0;
	NowYear  = 2021;

	// starting state (feel free to change this if you want):
	NowNumDeer = 2;
	NowHeight =  3.;

	enviroment();

	omp_set_num_threads( 4 );	// same as # of sections
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			Deer();
		}

		#pragma omp section
		{
			Grain();
		}

		#pragma omp section
		{
			Watcher();
		}

		#pragma omp section
		{
			MyAgent();	// your own
		}
	}       // implied barrier -- all functions must return in order
	// to allow any of them to get past here
}