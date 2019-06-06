#include <cstdlib>
#include <iostream>
#include "Matrix.h"
#include <pthread.h> 
#include <time.h>
#include <vector>

#include <ctime>
#include <ratio>
#include <chrono>

#define sizeT 7 

using namespace std;
using namespace std::chrono;

double dets[sizeT];
vector<int> data{9,4,6,2,5,4,3,1,5,1,10,1,3,3,3,5,8,3,3,6,8,5,2,8,5,8,7,6,5,9,9,1,4,1,8,3,4,8,8,10,7,5,6,7,6,2,3,7,6};

int threadNumber;
int numberOfThreads;

double getMean(vector<double> values) {
    double meanVal = 0;
    for (double value : values) {
        meanVal += value;
    }
    return (meanVal / values.size());
}

double getStdDev(vector<double> values) {
    double mean = getMean(values);
    double stdDev = 0;
    for (double value : values) {
        stdDev += (value - mean) * (value - mean);
    }
    return (sqrt(stdDev / values.size()));
}

double getDispersion(vector<double> values) {
    return (getStdDev(values) * getStdDev(values));
}

void* createTdOld(void* arg) 
{
    int *ar = (int *)arg;
    Matrix<int> testMat = Matrix<int>(sizeT, sizeT, data);
    dets[*ar] = dets[*ar] * testMat.submatrix(0,*ar).determinant();
}

void* createTd(void* arg) 
{    
    Matrix<int> testMat = Matrix<int>(sizeT, sizeT, data);
    for (int i = threadNumber; i < sizeT; i+=numberOfThreads){
        dets[i] = dets[i] * testMat.submatrix(0,i).determinant();
    }
}

double runThreads(int numThreads){
    int p[sizeT];
    pthread_t thread[numThreads];
    
    for (int i = 0; i < sizeT; i++) {
        dets[i] = data[i];
    }
    
    if(numThreads == 1) {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        threadNumber = 0;
        numberOfThreads = 1;
        pthread_create(&thread[0], NULL, &createTd, NULL);         
        pthread_join(thread[0], NULL);
        double detfin = 0;    
        for (int i = 0; i < sizeT; i++) { 
            if (i % 2 == 0) 
                detfin += dets[i]; 
            else
                detfin -= dets[i]; 
        }
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        return time_span.count();
    }
    else {
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        threadNumber = 0;
        numberOfThreads = numThreads;
        for(int i = 0; i < numThreads; i++) {
            pthread_create(&thread[i], NULL, &createTd, NULL);       
             
            threadNumber++;
        }    
        for(int i = 0; i < numThreads; i++) {
            pthread_join(thread[i], NULL);
        }
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        double detfin = 0;    
        for (int i = 0; i < sizeT; i++) { 
            if (i % 2 == 0) 
                detfin += dets[i]; 
            else
                detfin -= dets[i]; 
        }        
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        return time_span.count();
    }    
}

void testThreads(int numThreads) {
    vector<double> time;
    for (int i = 0; i < 200; i++){
        time.push_back(runThreads(numThreads));
    }
    double mean = getMean(time);
    double dispersion = getDispersion(time);
    cout << "Mean " << numThreads << " threads:" << mean << endl;
    cout << "Dispersion " << numThreads << " threads:" << dispersion << endl; 
}

int main(int argc, char** argv) {
    testThreads(1);
    testThreads(2);
    testThreads(4);
    testThreads(6);
    testThreads(8);
    testThreads(10);
    testThreads(12);
    return 0;
}

