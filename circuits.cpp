/*
* David Dray ~ COMP 233 ~ Spr 2020
* File:    circuits.c
* Purpose: Run millions of tests to check the
*       reliability of a set of circuits
*/

#define _CRT_SECURE_NO_WARNINGS //iso standard is good for us
//include statements
#include<stdio.h> 
#include<stdlib.h>
#include<omp.h>
#include<time.h>

// method stubs
void parallelTesting(int* myCircuits, int numCircuits, 
    double* startTime, double* stopTime);
void serialTesting(int* myCircuits, int numCircuits, 
    double* startTime, double* stopTime);
void altParallel(int* myCircuits, int numCircuits, 
    double* startTime, double* stopTime);

//Global Variables
const int NUM_RUNS = 1 << 28;   //run tests incrementing up to 2^28 times
const int NUM_THREADS = 2;      //number of threads for parallel section

int main() {
    //variable dictionary:
    const int numCircuits = 7;  //number of circuits
    //array of probabilities (for each circuit succeeding)
    int myCircuits[] = {98,92,89,91,93,88,90};
    double serialTime, parallelTime;    //time each testing type took
    double startTime, stopTime;     //timing variables

    //standard initial output
    printf("David Dray ~ COMP 233 B ~ Spring 2020\n");
    printf("Circuits - Run millions of tests to check the"
        "reliability of a set of circuits\n\n");


    printf("\Serial version:\n");
    //headers
    printf("\nnumber of tests:");
    printf("\tnumber of successes:");
    printf("\tpercent correct:\n");

#pragma omp parallel num_threads(NUM_THREADS)
    {
        srand((unsigned int)time(NULL) + omp_get_thread_num());
    }

    //run the serial testing function
    serialTesting(myCircuits, numCircuits, &startTime, &stopTime);

    //get time testing took
    serialTime = stopTime - startTime;
    printf("Time it took to run: %.3f", serialTime);

    /****************Parallel version*****************/

    printf("\nParallel version:\n\n");
    //headers
    printf("\nnumber of tests:");
    printf("\tnumber of successes:");
    printf("\tpercent correct:\n");

    //run the parallel testing function
    parallelTesting(myCircuits, numCircuits, &startTime, &stopTime);

    //print parallel testing time
    parallelTime = stopTime - startTime;
    printf("Time it took to run: %.3f\n", parallelTime);

    //prints speedup and efficiency
    printf("Speedup: %f\n", double(serialTime)/parallelTime);
    printf("Efficiency: %f\n", double((serialTime)/parallelTime)/NUM_THREADS);

    //standard termination message
    printf("\n\n\t<<Normal Termination>>\n\n");
    return 0;
}

/************************************************************************
* Function:    parallelTesting
* Purpose:     to test a set of circuits and see how often the whole
*           system will work (run several sets of tests, doubling the
*           size of the test each time).
*           This method uses 2 threads to more rapidly complete the tests
* Input args:  takes in the array of percentages for each circuit, the
*           number of circuits, and the pointers to the stop and start 
*           time variables which are used in main for timing
************************************************************************/
void parallelTesting(int* myCircuits, int numCircuits, 
    double* startTime, double* stopTime) {
    
    int numTests, curTest, i;   //loop variables
    int numSucc;    //number of successes
    //divides each section of the system into parts to check
    int section1, section2, section3, section4;

    //srand(time(NULL));   //seeds random generator

    *startTime = omp_get_wtime();
    //for loop to do each iteration of tests (16,32,etc)
    for (numTests = 16; numTests <= NUM_RUNS; numTests *= 2) {
        numSucc = 0;
        /*following your videos instructions didnt work for randomizing
        the inner for loops values (went back to values from previous
        time entering the loop. instead I made a random variable to
        seed each of the randoms inside the inner loop*/

        //create team of threads
#pragma omp parallel num_threads(NUM_THREADS) \
private(curTest, section1, section2, section3, section4)
        {
            int myID = omp_get_thread_num();    //which one

#pragma omp for reduction(+:numSucc)
            //parallel for loop for testing numTests times
            for (curTest = 0; curTest < numTests; curTest++) {

                //is circuit A working?
                section1 = myCircuits[0] > (rand() % 100);
                //is circuit B or C working?
                section2 = myCircuits[1] > (rand() % 100) ||
                    myCircuits[2] > (rand() % 100);

                //is circuit D or E working?
                section3 = myCircuits[3] > (rand() % 100) ||
                    myCircuits[4] > (rand() % 100);
                //is circuit F or G working?
                section4 = myCircuits[5] > (rand() % 100) ||
                    myCircuits[6] > (rand() % 100);
                //if one of the paths is working then system success
                if ((section1 && section2) || (section3 && section4)) {
                    numSucc++;  
                }
            }
        }
        //print out results for each group of tests
        printf("%10d %22d %28f\n", numTests, numSucc,
            double(numSucc) / numTests);
    }
    *stopTime = omp_get_wtime();    //stop time
}

/************************************************************************
* Function:    serialTesting
* Purpose:     to test a set of circuits and see how often the whole
*           system will work (run several sets of tests, doubling the
*           size of the test each time)
* Input args:  takes in the array of percentages for each circuit, the
*           number of circuits, and the pointers to the stop and start
*           time variables which are used in main for timing
************************************************************************/
void serialTesting(int* myCircuits, int numCircuits, 
    double* startTime, double* stopTime) {
    //divides each section of the system into different parts to check
    int section1, section2, section3, section4;
    int numTests, curTest, i;   //loop variables
    int numSucc;    //number of successes

    srand(time(NULL));

    *startTime = omp_get_wtime();   //start timing
    for (numTests = 16; numTests <= NUM_RUNS; numTests*=2) {
        numSucc = 0;
        curTest = 0;

        //for loop to run numTests number of tests
        for (curTest = 0; curTest < numTests; curTest++) {
            //is circuit A working?
            section1 = myCircuits[0] > (rand() % 100);
            //is circuit B or C working?
            section2 = myCircuits[1] > (rand() % 100) ||
                myCircuits[2] > (rand() % 100);

            //is circuit D or E working?
            section3 = myCircuits[3] > (rand() % 100) ||
                myCircuits[4] > (rand() % 100);
            //is circuit F or G working?
            section4 = myCircuits[5] > (rand() % 100) ||
                myCircuits[6] > (rand() % 100);
            //if one of the paths is working then system success
            if ((section1 && section2) || (section3 && section4)) {
                numSucc++;
            }
        }
        //print the row of number of tests, successes and success percent
        printf("%10d %22d %28f\n", numTests, numSucc,
            double(numSucc) / numTests);    
    }
    *stopTime = omp_get_wtime();    //stop timing
}












void altParallel(int* myCircuits, int numCircuits, double* startTime, double* stopTime) {
    unsigned int timeOfDay = time(NULL);

    *startTime = omp_get_wtime();
#pragma omp parallel num_threads(NUM_THREADS) \
shared(timeOfDay)
    {
        unsigned int timeOfDay = time(NULL);
        //divides each section of the system into different parts to check
        int section1, section2, section3, section4;
        int numTests, curTest, i;   //loop variables
        int numSucc;    //number of successes
        int myID = omp_get_thread_num();
        int numThreads = omp_get_num_threads();
        int myStart = 16 * (1 + myID);
        srand(timeOfDay + myID);   //seeds random generator

        for (numTests = myStart; numTests < NUM_RUNS; numTests *= 4) {
            numSucc = 0;
            for (curTest = 0; curTest < numTests; curTest++) {

                section1 = myCircuits[0] > (rand() % 100);
                section2 = myCircuits[1] > (rand() % 100) ||
                    myCircuits[2] > (rand() % 100);

                section3 = myCircuits[3] > (rand() % 100) ||
                    myCircuits[4] > (rand() % 100);
                section4 = myCircuits[5] > (rand() % 100) ||
                    myCircuits[6] > (rand() % 100);
                if ((section1 && section2) || (section3 && section4)) {
                    numSucc++;
                }
            }
            printf("%10d %22d %28f\n", numTests, numSucc,
                double(numSucc) / numTests);
        }
    }
    *stopTime = omp_get_wtime();


}