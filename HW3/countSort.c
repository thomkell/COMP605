#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
// #include <omp.h>


void Count_Sort(int a[] , int n)
{
    int i, j, count;
    int* temp = malloc(n*sizeof(int));

    //int thread_count = 10;

//#    pragma omp parallel num_threads(thread_count)
    for (i = 0; i < n; i++)
    {
        count = 0;
        for (j = 0; j < n; j++)
        {
            if (a[j] < a[i])
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        }
        temp[count] = a[i];

    }

    memcpy(a, temp, n*sizeof(int));
    free (temp) ;
}

// https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
int cmpfunc(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

int main(int argc, char* argv[]) {

    srand(45);

    // int a[] = {9,203,77,345,4,8,90,90};
    int arraySize = 10000; //strtol(argv[1], NULL, 10);
    int a[arraySize];
    int b[arraySize];
    srand(0);

    for (int i = 0; i < arraySize; i++) {
        a[i] = rand()%1000;
        printf("a[%i] = %i\n", i, a[i]);
        b[i] = a[i];
    }

    int n = sizeof(a) / sizeof(a[0]); //sizeof returns the size size of given expression

    // start time measurement
    struct timeval time1, time2, time3, time4;
    gettimeofday(&time1, NULL);

    // call function
    Count_Sort(a, n);

    gettimeofday(&time2, NULL);

    for(int i = 0; i < arraySize; i++)
    {
        printf("%i, ", a[i]);
    }
    printf("\n");

    gettimeofday(&time3, NULL);
    // call  qsort
    qsort(b, n, sizeof(int), cmpfunc);

    gettimeofday(&time4, NULL);


    double totalTime;
    double totalTime2;
    // calculate total time for count sort in ms
    totalTime = (time2.tv_sec - time1.tv_sec);      // s
    totalTime += (time2.tv_usec - time1.tv_usec)/1E6; //ms

    // calculate total time for qsort in ms
    totalTime2 = (time4.tv_sec - time3.tv_sec);      // s
    totalTime2 += (time4.tv_usec - time3.tv_usec)/1E6; //ms

    printf("time count sort %fs\n", totalTime);
    printf("time qsort %fs\n", totalTime2);

    return 0;
}
