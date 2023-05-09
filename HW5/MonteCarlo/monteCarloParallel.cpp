// Thomas Keller
// COMP605 Assignment 5 Problem 1
// Monte Carlo method, Pi

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/time.h>
#include <upcxx/upcxx.hpp>
#include <chrono>
#include <ctime>
using namespace std;

// Global variables
long long int local_number_in_circle = 0;

// Function declaration
void CalcToss(int myRank, int comm_sz, int numberTossesThread);

int main(int argc, char* argv[])
{
    // Using time point and system_clock
    std::chrono::time_point<std::chrono::system_clock> start, end;

    int comm_sz; //number of processes
    int myRank; // process rank

    long long int number_of_tosses;
    long long int numberTossesThread;
    long long int numberTosses;

    upcxx::init();
    comm_sz = upcxx::rank_n();
    myRank = upcxx::rank_me();

    upcxx::barrier();

    if(upcxx::rank_me() == 0)
    {
        number_of_tosses = atoll(argv[1]); //convert string to long long int
	numberTosses = pow(10, number_of_tosses);
        // number of tosses per rank
        numberTossesThread = numberTosses / comm_sz;
	start = std::chrono::system_clock::now();
    }
    // Broadcast the number_of_tosses to all processes
    upcxx::broadcast(&numberTossesThread, 1 /*Count*/, 0 /*Root*/).wait();

    // call function CalcToss
    CalcToss(myRank, comm_sz, numberTossesThread);

    // sum the number_in_circle values from all processes
    long long int global_number_in_circle;
    global_number_in_circle = upcxx::reduce_all(local_number_in_circle, upcxx::op_fast_add).wait();

    upcxx::barrier();
    end = std::chrono::system_clock::now();

    if(upcxx::rank_me() == 0)
    {
	cout<<"total number of tosses: "<<global_number_in_circle<<endl;
        // calculate Pi
        double pi_estimate;
        pi_estimate = 4 * (global_number_in_circle / ((double) numberTosses));
	cout <<"Estimation of Pi = " <<pi_estimate<<endl;
	std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

        }

    upcxx::finalize();

    return 0;
}

// Calculate number of tosses
void CalcToss(int myRank, int comm_sz, int numberTossesThread)
{
    // seeds random number generator, randomize rand()
    srand(upcxx::rank_me());

    cout<<"number of porcesses: "<<comm_sz<<" myRank: "<<myRank<<endl;

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
}

