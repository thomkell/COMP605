// Thomas Keller
// COMP605 Assignment 4 Problem 1
// Monte Carlo method, Pi

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>

// Global variables
long long int local_number_in_circle = 0;

// Function declaration
void CalcToss(int myRank, int comm_sz, int numberTossesThread);

int main(int argc, char* argv[])
{
    int comm_sz; //number of processes
    int myRank; // process rank

    long long int number_of_tosses;
    long long int numberTossesThread;
    long long int numberTosses;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if(myRank == 0)
    {
        number_of_tosses = atoll(argv[1]); //convert string to long long int
    numberTosses = pow(10, number_of_tosses);
        // number of tosses per rank
        numberTossesThread = numberTosses / comm_sz;
    }

    double local_start, local_finish, local_elapsed, elapsed;
    MPI_Barrier(MPI_COMM_WORLD);
    local_start = MPI_Wtime();

    // Broadcast the number_of_tosses to all processes
    MPI_Bcast(&numberTossesThread, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // call function CalcToss
    CalcToss(myRank, comm_sz, numberTossesThread);

    // sum the number_in_circle values from all processes
    long long int global_number_in_circle;
    MPI_Reduce(&local_number_in_circle, &global_number_in_circle, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // calc time
    local_finish = MPI_Wtime();
    local_elapsed = local_finish - local_start;
    MPI_Reduce(&local_elapsed , &elapsed , 1 , MPI_DOUBLE, MPI_MAX, 0 , MPI_COMM_WORLD);
    // MPI_Finalize();

    if(myRank == 0)
    {
        // calculate Pi
        double pi_estimate;
        pi_estimate = 4 * (global_number_in_circle / ((double) numberTosses));
    printf("Estimation of Pi = %lf\n", pi_estimate);

    printf("Elapsed time = %e seconds\n" , elapsed);

        }

    MPI_Finalize();

    return 0;
}

// Calculate number of tosses
void CalcToss(int myRank, int comm_sz, int numberTossesThread)
{
    // seeds random number generator, randomize rand()
    srand( time ( NULL));

    printf("number of processes: %i , myRank: %i \n", comm_sz, myRank);

    double x;
    double y;
    double distance_squared;

    for(long long int toss = 0; toss < numberTossesThread; toss++)
    {
        x = (double) rand() / RAND_MAX * 2.0 - 1.0; //Dividing rand() by RAND_MAX is value [0,1], multiply by 2 and sub 1 -> [-1,1]
        y = (double) rand() / RAND_MAX * 2.0 - 1.0;

        distance_squared = x * x + y * y;

        if (distance_squared <= 1) {
            local_number_in_circle++;
        }

    }
    // MPI_Barrier(MPI_COMM_WORLD);
}

