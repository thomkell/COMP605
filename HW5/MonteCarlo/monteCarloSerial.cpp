// Thomas Keller
// COMP605 Assignment 5 Problem 1
// Monte Carlo method, Pi

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/time.h>

using namespace std;

// Global variables
int thread_count;
long long int number_in_circle = 0;
long long int numberTossesThread;

// Function declaration
void CalcToss();

int main(int argc, char* argv[])
{
    // declare time measurement var
    struct timeval time1, time2;
    // struct rusage time1, time2;

    // threads & read commandline inputs argv
    thread_count = 1;//strtol(argv[1], NULL, 10);
    long long int tosses = strtol(argv[1], NULL, 10);
    long long int number_of_tosses = pow(10,tosses);

    //thread_handles = malloc(thread_count*sizeof(pthread_t));

    numberTossesThread = number_of_tosses/thread_count;

    // start time measurement
    double totalTime;
    gettimeofday(&time1, NULL);

    // call function calcToss
    CalcToss();

    // stop time measurement
    gettimeofday(&time2, NULL);

    // calculate Pi
    double pi_estimate;
    pi_estimate = 4 * (number_in_circle / ((double) number_of_tosses));
    cout <<"Estimation of Pi = " <<pi_estimate<<endl;


    // calculate total time in ms
    totalTime = (time2.tv_sec - time1.tv_sec) * 1E3;      // seconds to ms
    totalTime += (time2.tv_usec - time1.tv_usec) / 1E3;   // us to ms
    cout <<totalTime <<"ms"<<endl;

    return 0;
}

// Calculate number of tosses
void CalcToss()
{
    // seeds random number generator, randomize rand()
    srand( time ( NULL));

    //long my_rank = (long) rank;
    double x;
    double y;
    double distance_squared;
    long long temp = 0;

    for(long long int toss = 0; toss < numberTossesThread; toss++)
    {
        x = (double) rand() / RAND_MAX * 2.0 - 1.0; //Dividing rand() by RAND_MAX is value [0,1], multiply by 2 and sub 1 -> [-1,1]
        y = (double) rand() / RAND_MAX * 2.0 - 1.0;

        distance_squared = x * x + y * y;

        if (distance_squared <= 1) {
            temp++;
        }
    }

    number_in_circle += temp;
    cout<<"number in circle = " <<number_in_circle<<endl;

}

