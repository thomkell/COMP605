// Thomas Keller, Assignment 4, Problem 2
// Merge Sort Algorithm, parallized with MPI
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>
#include <math.h>
#include <time.h>

// references: https://www.geeksforgeeks.org/merge-sort/
// merge Arrays
int* mergeArray(int* arr1, int n1, int* arr2, int n2) {
    int* result = (int*)malloc((n1 + n2) * sizeof(int));
    int i = 0, j = 0, k = 0;

    while (i < n1 && j < n2) {
        if (arr1[i] < arr2[j]) {
            result[k++] = arr1[i++];
        } else {
            result[k++] = arr2[j++];
        }
    }

    while (i < n1) {
        result[k++] = arr1[i++];
    }

    while (j < n2) {
        result[k++] = arr2[j++];
    }
    return result;
}

// merge sort function, splitting array recursive
void mergeSort(int array[], int firstElement, int lastElement)
{
    if(firstElement < lastElement)
    {
        int midElement = firstElement + (lastElement - firstElement) / 2;

        mergeSort(array, firstElement, midElement);
        mergeSort(array, midElement + 1, lastElement);

        int* M = mergeArray(array + firstElement, midElement - firstElement + 1, array + midElement + 1, lastElement - midElement);
        for(int i = 0; i < lastElement - firstElement + 1; i++)
        {
             array[firstElement + i] = M[i];
        }
    free(M);
    }
}

int main(int argc, char** argv) {

    // Init MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    // time measurement variables
    double local_start , local_finish , local_elapsed , elapsed ;
    MPI_Barrier(MPI_COMM_WORLD);

    // set seed for rand
    srand(clock());

    // start time
    local_start = MPI_Wtime();

    // read amount of numbers to sort
    int n = (int)pow(10,strtol(argv[1], NULL, 10));
    int* receiveArray;
    int* localArray;
    int chunkSize = n / size;
    localArray = (int*)malloc(chunkSize * sizeof(int));

    for(int i = 0; i < chunkSize; i++)
    {
    localArray[i] = rand() % 1000;
    }

    // sort local array
    mergeSort(localArray, 0, chunkSize - 1);

    // tree structure communication
    for (int mergeStep = 1; mergeStep < size; mergeStep *= 2)
    {
    // RECEIVE
    if(rank % (2*mergeStep) == 0)
    { 
             // printf("Processor %d receiving from Processor %d\n", rank, rank + mergeStep);
         // allocate memory
         receiveArray = (int *)malloc(chunkSize * mergeStep * sizeof(int));
         // reveive array to merge
         MPI_Recv(receiveArray, chunkSize*mergeStep, MPI_INT, rank+mergeStep, 0, MPI_COMM_WORLD, &status);
         // merge arrays, localArray and receiveArray
         localArray = mergeArray(localArray, chunkSize*mergeStep, receiveArray, chunkSize*mergeStep);
    }
    // SEND
    else
    {
         // int next = rank - mergeStep;
         // printf("Processor %d sending to Processor %d\n", rank, next);
         // send array, localArray
         MPI_Send(localArray, chunkSize*mergeStep, MPI_INT, rank-mergeStep, 0, MPI_COMM_WORLD);
         break;
    }
    }

    // stop time
    local_finish = MPI_Wtime() ;
    local_elapsed = local_finish - local_start ;

    // sum time
    MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    MPI_Finalize();

    if(rank == 0)
    {
         //print array after merging
         printf("Array is: \n");
         for (int i = 0; i < n; i++)
         {
             printf("%d ", localArray[i]);
         }
     printf("\n");
    printf ("Elapsed time = %e seconds\n" , elapsed ) ;
    }

    return 0;

}
