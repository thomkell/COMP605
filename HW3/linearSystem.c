// Thomas Keller, Assign3 Problem 3 - Column major
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// #include <omp.h>

// Backsubtitution Col
void backSubCol(int numberOfThreads, int n, double *b, double *x, double *matrixA[]){
#   pragma omp parallel for num_threads(numberOfThreads)
    for(int row = 0; row < n; row++)
    {
        x[row] = b[row];
    }
#   pragma omp single
    for(int col = n-1; col >=0; col--)
    {
        x[col] /= matrixA[col][col];
#       pragma omp parallel for num_threads(numberOfThreads) \
        schedule(runtime)
        for (int row = 0; row < col; row++)
        {
            x[row] -= matrixA[row][col]*x[col];
            //printf("Thread: %i\n", omp_get_thread_num());
        }
    }
}


int main(int argc, char* argv[]) {

    // seed random
    srand(0);

    // read & set number of Threads
    int numberOfThreads = strtol(argv[1], NULL, 10);

    // define matrix
    int n = strtol(argv[2], NULL, 10);

    // allocate memory for matrixA
    double** matrixA = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++)
        matrixA[i] = (double*)malloc(n * sizeof(double));

    // initialize 0 to matrix A
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrixA[i][j] = 0;
        }
    }

    // add rand numbers to matrix upper triangular matrix
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            matrixA[i][j] =  (double) rand() / RAND_MAX; //scale double
        }
    }

