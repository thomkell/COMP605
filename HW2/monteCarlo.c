// Thomas Keller
// COMP605 Assignment 2 Problem 1
// Monte Carlo method, Pi

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>

// Global variables
int thread_count;
long long int number_in_circle = 0;
long long int numberTossesThread;

// Function declaration
void* CalcToss(void* rank);

int main(int argc, char* argv[])
{
    // start time measurement
    struct timeval time1, time2;
    // struct rusage time1, time2;
    double totalTime;
    //getrusage(RUSAGE_SELF, &time1);
    gettimeofday(&time1, NULL);

    // threads & read commandline inputs argv
    long    thread;
    pthread_t* thread_handles;
    thread_count = 10;//strtol(argv[1], NULL, 10);
    long long int number_of_tosses = 10000000;//strtol(argv[2], NULL, 10);

    thread_handles = malloc(thread_count*sizeof(pthread_t));

    numberTossesThread = number_of_tosses/thread_count;

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, CalcToss, (void *) thread);
    }

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    // calculate Pi
    double pi_estimate;
    pi_estimate = 4 * (number_in_circle / ((double) number_of_tosses));
    printf("Estimation of Pi = %lf\n", pi_estimate);

    free(thread_handles);

    // stop time measurement
    //getrusage(RUSAGE_SELF, &time2);
    gettimeofday(&time2, NULL);

    // calculate total time in ms
    // totalTime = (time2.ru_utime.tv_sec - time1.ru_utime.tv_sec) * 1E3;      // seconds to ms
    // totalTime += (time2.ru_utime.tv_usec - time1.ru_utime.tv_usec) / 1E3;   // us to ms
    totalTime = (time2.tv_sec - time1.tv_sec) * 1E3;      // seconds to ms
    totalTime += (time2.tv_usec - time1.tv_usec) / 1E3;   // us to ms
    printf("%fms\n", totalTime);

    return 0;
}

// Calculate number of tosses
void* CalcToss(void* rank)
{
    // seeds random number generator, randomize rand()
    srand( time ( NULL));

    long my_rank = (long) rank;
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
    printf("number in circle = %lld, my_rank = %i\n", number_in_circle, my_rank);

    return 0;
}
