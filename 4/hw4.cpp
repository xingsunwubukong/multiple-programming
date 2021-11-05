#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


int    NowYear;        // 2021 - 2026
int    NowMonth;        // 0 - 11

float    NowPrecip;        // inches of rain per month
float    NowTemp;        // temperature this month
float    NowHeight;        // grain height in inches
int      NowNumDeer;        // number of deer in the current population

void     Deer();
void     Grain();
void     Watcher();
void     MyAgent();

int      KillerWolf;
int      Wolves;

const float GRAIN_GROWS_PER_MONTH =        9.0;
const float ONE_DEER_EATS_PER_MONTH =        1.0;

const float AVG_PRECIP_PER_MONTH =        7.0;    // average
const float AMP_PRECIP_PER_MONTH =        6.0;    // plus or minus
const float RANDOM_PRECIP =            2.0;    // plus or minus noise

const float AVG_TEMP =                60.0;    // average
const float AMP_TEMP =                20.0;    // plus or minus
const float RANDOM_TEMP =            10.0;    // plus or minus noise

const float MIDTEMP =                40.0;
const float MIDPRECIP =                10.0;

unsigned int seed = 0;

float
SQR( float x )
{
        return x*x;
}

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

        return (int)(  Ranf(seedp, low, high) );
}

int main() {
    // starting date and time:
    NowMonth = 0;
    NowYear = 2021;

    // starting state (feel free to change this if you want):
    NowNumDeer = 2;
    NowHeight  = 3.;

    KillerWolf = 0;

    float ang = (30. * (float)NowMonth + 15.) * (M_PI / 180.);

    float temp = AVG_TEMP - AMP_TEMP * cos(ang);
    NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP);

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
    NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);
    if (NowPrecip < 0.)
        NowPrecip = 0.;

    omp_set_num_threads(4);    // same as # of sections
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
            MyAgent();    // sun code~~
        }
    }      
    // implied barrier -- all functions must return in order
    // to allow any of them to get past here
}

void Deer(){
    while(NowYear < 2027){
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        int nextNumDeer = NowNumDeer;
        int carryingCapacity = (int)( NowHeight );
        if( nextNumDeer < carryingCapacity )
                nextNumDeer++;
        else{
            if( nextNumDeer > carryingCapacity ){
                nextNumDeer--;
            }
        }
        nextNumDeer += KillerWolf;

        if( nextNumDeer < 0 )
                nextNumDeer = 0;
       

        // DoneComputing barrier:
        #pragma omp barrier
        NowNumDeer = nextNumDeer;

        // DoneAssigning barrier:
        #pragma omp barrier
        // nothing~

        // DonePrinting barrier:
        #pragma omp barrier
        // nothing~
        
    }
}

void Grain(){
    while(NowYear < 2027){
        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );
        float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
        
        float nextHeight = NowHeight;
        nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
        nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
        if( nextHeight < 0. ) nextHeight = 0.;

        // DoneComputing barrier:
        #pragma omp barrier
        NowHeight = nextHeight;

        // DoneAssigning barrier:
        #pragma omp barrier
        // nothing~

        // DonePrinting barrier:
        #pragma omp barrier
        // nothing~
        
    }
}


void Watcher(){
    while(NowYear < 2027){

        // compute a temporary next-value for this quantity
        // based on the current state of the simulation:
        
        // DoneComputing barrier:
        #pragma omp barrier
        // nothing~

        // DoneAssigning barrier:
        #pragma omp barrier
        float Temp    =    (5./9.) * (NowTemp - 32);

        float Prec    =    NowPrecip * 2.54;

        float Height  =    NowHeight * 2.54;

        printf("  %2d Year:    %2d Month     %2d Deers     GrainHeight: %5.2lf     Precip: %5.2lf     Temp: %5.2lf     Wolves: %d\n", NowYear, (NowMonth + 1), NowNumDeer, Height, Prec, Temp, Wolves);

        if(NowMonth >= 11){
            NowMonth = 0;
            NowYear++;
        }
        else{
            NowMonth++;
        }
        
        float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

        float temp = AVG_TEMP - AMP_TEMP * cos( ang );
        NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );

        float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
        NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
        if( NowPrecip < 0. )
            NowPrecip = 0.;
        
     
        // DonePrinting barrier:
        #pragma omp barrier
        // nothing~
    }
}

void MyAgent() {
    while (NowYear < 2027) {

        int leftovers;
        //When the number of deer is too high, more than twice the height of the grain, wolves will find them
        if (NowNumDeer > NowHeight*2) {
            leftovers = -1 * leftovers/2;
        }
        else
            leftovers = 2;

        if (Wolves = NowNumDeer) {
            Wolves++;
      } 
        else if (Wolves > NowNumDeer) {
            Wolves--;
        }
        else if (Wolves < NowNumDeer) {
            Wolves++;
        }

  
#pragma omp barrier
        KillerWolf = leftovers;


#pragma omp barrier

#pragma omp barrier

    }
}


