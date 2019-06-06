#include <iostream>
#include <functional>
#include <vector>

#include <omp.h>
#include "Matrix.h"

#include <time.h>

#include <ctime>
#include <ratio>
#include <chrono>

using namespace std;

double getMean(vector<double> values) {
    double meanVal = 0;
    for (double value : values) {
        meanVal += value;
    }
    return meanVal / values.size();
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

void runThreads(int numThreads) {
    using namespace std::chrono;
    vector<int> data{ 9,4,6,2,5,4,3,1,5,1,10,1,3,3,3,5,8,3,3,6,8,5,2,8,5,8,7,6,5,9,9,1,4,1,8,3,4,8,8,10,7,5,6,7,6,2,3,7,6 };
    Matrix<int> testMat = Matrix<int>(7, 7, data);
    vector<double> times;
    for (int i = 0; i < 200; i++) {
        omp_set_dynamic(0);     // Explicitly disable dynamic teams
        omp_set_num_threads(numThreads);
        //clock_t tStart1 = clock();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        double det = testMat.determinant();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        times.push_back(time_span.count());
    }

    double mean = getMean(times);
    double dispersion = getDispersion(times);
    cout << "Mean " << numThreads << " threads:" << mean << endl;
    cout << "Dispersion " << numThreads << " threads:" << dispersion << endl;
}

void runSimple(int numThreads) {
    using namespace std::chrono;
    vector<int> data{ 9,4,6,2,5,4,3,1,5,1,10,1,3,3,3,5,8,3,3,6,8,5,2,8,5,8,7,6,5,9,9,1,4,1,8,3,4,8,8,10,7,5,6,7,6,2,3,7,6 };
    Matrix<int> testMat = Matrix<int>(7, 7, data);
    vector<double> times;
    for (int i = 0; i < 200; i++) {
        //clock_t tStart1 = clock();
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        double det = testMat.determinant();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        times.push_back(time_span.count());
    }

    double mean = getMean(times);
    double dispersion = getDispersion(times);
    cout << "Mean " << numThreads << " threads:" << mean << endl;
    cout << "Dispersion " << numThreads << " threads:" << dispersion << endl;
}

int main()
{
    
    runThreads(1);
    runThreads(2);
    runThreads(4);
    runThreads(6);
    runThreads(8);
    runThreads(10);
    runThreads(12);
    
    //runSimple(1);
    getchar();

    return 0;
}


