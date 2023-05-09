// Thomas Keller
// COMP605, Assignment 5, Problem 2
// merge sort serial
// How to run: qsub batch.serial or ./mainS <exponent power of 2>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/time.h>
#include <chrono>
#include <ctime>

using namespace std;

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

    // set rand seed
    srand(clock());

    // Using time point and system_clock
    chrono::time_point<std::chrono::system_clock> start, end;


    int n = (int)pow(2,strtol(argv[1], NULL, 10));

    // init array and add random values
    int array[n];
    for (int i = 0; i < n; i++)
    {
        array[i] = rand()% 1000;
    }

    // calc array size
    int arr_size = sizeof(array) / sizeof(array[0]);

    // start time measurement
    start = chrono::system_clock::now();

    // start merging
    mergeSort(array, 0, arr_size - 1);

    // stop time measurement
    end = chrono::system_clock::now();

    // print array after merging
    cout<<"Sorted array is: \n";
    for (int i = 0; i < arr_size; i++)
    {
        cout<<array[i]<<" ";
     }
    cout<<"\n";

    // calculate total time
    chrono::duration<double> elapsed_seconds = end - start;
    time_t end_time = chrono::system_clock::to_time_t(end);

    cout << "finished computation at " << ctime(&end_time)
         << "elapsed time: " << elapsed_seconds.count() << "s\n";

    return 0;
}

