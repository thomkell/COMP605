// Thomas Keller
// COMP605 Assignment 2 Problem 2
// Monte Carlo method, Trapezoidal

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
// #include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>

// global variables
int thread_count;
int flag = 0;
double result;
pthread_mutex_t mutex;
// sem_t semaphore;

// function declaration
void* trapezoidal(void* rank);

int main(int argc, char* argv[])
{

    // init threads and read commandline
    long    thread;
    pthread_t* thread_handles;
    thread_count = 10;//strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count*sizeof(pthread_t));

    // init semaphore
    // sem_init(&semaphore, 0, 1);

    // init mutex
    // int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
    // pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    // start time measurement
    struct rusage time1, time2;
    long double totalTime;
    getrusage(RUSAGE_SELF, &time1);

    //struct timeval time1, time2;
    //double totalTime;

    // gettimeofday(&time1, NULL);

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread], NULL, trapezoidal, (void *) thread);
    }

    // stop time measurement
    getrusage(RUSAGE_SELF, &time2);

    //gettimeofday(&time2, NULL);

    for (thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    printf("Result for Trapezoidal is %f, calculated by %d threads\n", result, thread_count);

    free(thread_handles);

    // sem_destroy(&semaphore);


    // calculate total time in ms
    totalTime = (time2.ru_utime.tv_usec - time1.ru_utime.tv_usec);      // seconds to ms
    totalTime += (time2.ru_utime.tv_usec - time1.ru_utime.tv_usec) / 1E3;   // us to ps
    //totalTime = (time2.tv_sec - time1.tv_sec) * 1E3;	  // seconds to ms
    //totalTime += (time2.tv_usec - time1.tv_usec) / 1E3;   // us to ms

    printf("%fus\n", totalTime);

    return 0;
}

// Function to calculate part of Integral depend on number of threads
void* trapezoidal(void* rank) {

    int a = 1;
    double b = exp(4);
    int n = 524288;//1024; // 2^n
    double sum = 0;

    double h = (b*a)/n;

    // split interval in that each thread has the "same" workload
    long my_rank = (long) rank;
    long double my_n = n/thread_count;
    long double my_a = a+h*my_rank*my_n;
    long double my_b = my_a + h*my_n;

    for(int k = 1; k < my_n-1; k++)
    {
        sum += log(my_a+k*((my_b-my_a)/my_n));
    }

    //pthread_mutex_lock(&mutex);
    // busy waiting loop
    while(flag != my_rank);
    printf("my_n= %f, my_a = %f , my_b = %f, my_rank = %i\n", my_n, my_a, my_b, my_rank);

    // sem_wait(&semaphore);

    // Compute Integral, ATTENTION: shared variable!!
    result += ((my_b - my_a) / my_n) * ((log(my_a) / 2) + sum + (log(my_b) / 2));
    flag = (flag + 1) % thread_count;
    //pthread_mutex_unlock(&mutex);

    // sem_post(&semaphore);

    return 0;
}
