/*
* David Dray ~ COMP 233 ~ Spr 2020
* File:    circuits.c
* Purpose: test reliability of a set of circuits
*/

#define _CRT_SECURE_NO_WARNINGS //iso standard is good for us
//include statements
#include<stdio.h> 
#include<stdlib.h>
#include<omp.h>
#include<time.h>

// method stubs
void parallelTesting(int* myCircuits, int numCircuits, double* startTime, double* stopTime);
void serialTesting(int* myCircuits, int numCircuits, double* startTime, double* stopTime);

//Global Variables
const int NUM_RUNS = 1 << 28;   //run 256 million times

int main() {
    printf("%d\n", NUM_RUNS);
    //variable dictionary:
    unsigned int timeOfDay = time(NULL);    //time for seeding
    const int numCircuits = 7;  //number of circuits
    int i;  //loop variable
    int systemWorks;    //boolean for if the system works or not
    //divides each section of the system into different parts to check
    int section1, section2, section3, section4;
    //array of probabilities
    int* myCircuits = (int*)malloc(numCircuits * sizeof(int));
    int curTest;    //current test
    int numTests;   //number of tests being run
    int numSucc;    //number of successes
    double startTime, stopTime;     //timing variables

    //standard initial output
    printf("David Dray ~ COMP 233 B ~ Spring 2020\n");
    printf("Circuits - Test reliability of a set of circuits\n");

    //give array the proper values
    myCircuits[0] = 98; //probability of A working
    myCircuits[1] = 92; //probability of B working
    myCircuits[2] = 89; //probability of C working
    myCircuits[3] = 91; //probability of D working
    myCircuits[4] = 93; //probability of E working
    myCircuits[5] = 88; //probability of F working
    myCircuits[6] = 90; //probability of G working

    //srand(timeOfDay);   //seeds random generator

    printf("\Serial version:\n");
    //headers
    printf("\nnumber of tests:");
    printf("\tnumber of successes:");
    printf("\tpercent correct:\n");
    serialTesting(myCircuits, numCircuits, &startTime, &stopTime);
    printf("Time it took to run: %.3f", stopTime - startTime);







    /****************Parallel version*****************/
    printf("\nParallel version:\n\n");
    //headers
    printf("\nnumber of tests:");
    printf("\tnumber of successes:");
    printf("\tpercent correct:\n");
    parallelTesting(myCircuits, numCircuits, &startTime, &stopTime);
    printf("Time it took to run: %.3f", stopTime - startTime);




    free(myCircuits);   //free memory

    //standard termination message
    printf("\n\n\t<<Normal Termination>>\n\n");
    return 0;
}


void parallelTesting(int * myCircuits, int numCircuits, double * startTime, double * stopTime) {
    unsigned int timeOfDay = time(NULL);

    *startTime = omp_get_wtime();
#pragma omp parallel num_threads(2) \
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

        for (numTests = myStart; numTests <= NUM_RUNS; numTests *= 4) {
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



void serialTesting(int* myCircuits, int numCircuits, double* startTime, double* stopTime) {
    //divides each section of the system into different parts to check
    int section1, section2, section3, section4;
    int numTests, curTest, i;   //loop variables
    int numSucc;    //number of successes

    numTests = 16;
    *startTime = omp_get_wtime();
    while (numTests <= NUM_RUNS) {
        numSucc = 0;
        curTest = 0;
        while (curTest < numTests) {
            

            section1 = myCircuits[0] > (rand()%100);
            section2 = myCircuits[1] > (rand() % 100) ||
                myCircuits[2] > (rand() % 100);

            section3 = myCircuits[3] > (rand() % 100) ||
                myCircuits[4] > (rand() % 100);
            section4 = myCircuits[5] > (rand() % 100) ||
                myCircuits[6] > (rand() % 100);
            if ((section1 && section2) || (section3 && section4)) {
                numSucc++;
            }
            curTest++;
        }
        printf("%10d %22d %28f\n", numTests, numSucc,
            double(numSucc) / numTests);
        numTests *= 2;
    }
    *stopTime = omp_get_wtime();
}