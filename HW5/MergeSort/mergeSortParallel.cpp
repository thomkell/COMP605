// Thomas Keller, Assignment 5, Problem 2
// Merge Sort Algorithm, parallized with upc++
// How to run: qsub batch.parallel or upcxx-run -n X ./main <exponent power of 2>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/time.h>
#include <chrono>
#include <ctime>
#include <upcxx/upcxx.hpp>

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
    for(int i = 0; i < n1+n2; i++)
    {
		arr1[i] = result[i];
	}
    return arr1;
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
    }
}

int main(int argc, char** argv) {

    // Init upc++
    upcxx::init();

    int rank, size;
    size = upcxx::rank_n();
    rank = upcxx::rank_me();

    // Using time point and system_clock
    chrono::time_point<chrono::system_clock> start, end;

    // set seed for rand
    srand(upcxx::rank_me());


    int chunkSize;

	if(rank == 0)
	{
		// read amount of numbers to sort
		int n = (int)pow(2,strtol(argv[1], NULL, 10));
		int* receiveArray;
		int* c;
		chunkSize = n / size;
	}

    // Broadcast chunksize
    upcxx::broadcast(&chunkSize, 1, 0).wait();

    // set up the distributed object
    auto localData = upcxx::new_array<int>(chunkSize*size);
    upcxx::dist_object<upcxx::global_ptr<int>>n(localData);

    int *localArray = localData.local();

    for(int i = 0; i < chunkSize; i++)
    {
	localArray[i] = rand() % 1000;
    }

    upcxx::barrier();
    // first time stamp
    start = chrono::system_clock::now();

    // sort local array
    mergeSort(localArray, 0, chunkSize - 1);

    // print local array
    //printf("local Array is: \n");
    //for (int i = 0; i < chunkSize; i++)
    //{
	//printf("%d ", localArray[i]);
    //}
    //printf("\n\n");

    // tree structure communication
    for (int mergeStep = 1; mergeStep < size; mergeStep *= 2)
    {
		upcxx::barrier();

		// RECEIVE
		if(rank % (2*mergeStep) == 0)
		{
			// create global ptr for received data
			upcxx::global_ptr<int> receiveData = upcxx::new_array<int>(chunkSize*mergeStep);
			int *receiveArray = receiveData.local();
			int neigbour = rank + mergeStep;
			auto next = n.fetch(neigbour).wait();

			// reveive array to merge
			upcxx::rget(next, receiveArray, chunkSize*mergeStep).wait();

			// merge arrays, localArray and receiveArray
			localArray = mergeArray(localArray, chunkSize*mergeStep, receiveArray, chunkSize*mergeStep);

			//print array after merging
			//printf("merged part array from rank %i is: \n", rank);
			//for (int i = 0; i < chunkSize*mergeStep*2; i++)
			//{
				//printf("%d ", localArray[i]);
			//}
			//printf("\n\n");
		}
    }

    // second time stamp
    upcxx::barrier();
    end = chrono::system_clock::now();

    upcxx::finalize();

    if(rank == 0)
    {
        //print array after merging
        printf("Array is: \n");
        for (int i = 0; i < chunkSize*size; i++)
        {
	    printf("%d ", localArray[i]);
        }
	printf("\n\n");

	// calc and show elapsed time
	chrono::duration<double> elapsed_seconds = end - start;
        time_t end_time = chrono::system_clock::to_time_t(end);

	cout << "finished computation at " << ctime(&end_time)
        << "elapsed time: " << elapsed_seconds.count() << "s\n";

    }

    return 0;

}
