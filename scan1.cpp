#include <iostream>
#include <cassert>
#include <omp.h>
#include <cstdlib>

using namespace std;

double* scan3(double*, int);
double* scan1(double*, int);
void scan_up(double*, double*, int, int, double& sum);
void scan_down(
        double, 
        double*, 
        double*, 
        double*, 
        int, 
        int);

template <typename T>
void printvec(T*, int);

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Usage: scan seed length n_threads" << endl;
        return -1;
    } 

    srand(atoi(argv[1])); 
    int length = atoi(argv[2]);
    int n_threads = atoi(argv[3]);
    omp_set_num_threads(n_threads); 
    
    double* vec = new double[length];
    for (int i = 0; i < length; i++) 
        vec[i] = rand() % length;
    
    double start = omp_get_wtime();
    double* scanned3 = scan3(vec, length);
    double pscan_time = omp_get_wtime() - start;
   	
    double* scanned1 = scan1(vec, length);
    for (int i = 0; i < length; i++) {
        assert(scanned3[i] == scanned1[i]);
    }
    
	cout << n_threads << ','
		 << length << ','
		 << pscan_time << endl;
}

double* scan3(double* x, int n) {
    double* y = new double[n];
    double* t = new double[n];

    y[0] = x[0];

    double sum;
    if (n > 1) {
        #pragma omp parallel
		#pragma omp single
        {
            scan_up(x, t, 1, n, sum);
            scan_down(x[0], x, t, y, 1, n);
        }
    }
    return y;
}

double* scan1(double* x, int n) {;
    double sum = 0;
    double* y = new double[n];

    for (int i = 0; i < n; i++) {
        sum += x[i];
        y[i] = sum;
    }

    return y;
}

void scan_up(double* x, double* t, int i, int j, double& sum) {
    if (i == j)
        sum = x[i];
    else {
        int k = (i+j)/2;
        #pragma omp task
        scan_up(x,t,i,k, t[k]);
        double right;
        scan_up(x,t,k+1,j, right);
        #pragma omp taskwait
        sum = t[k] + right;
    }
}

void scan_down(double v, 
        double* x, 
        double* t, 
        double* y, 
        int i, int j) {
    if (i == j)
        y[i] = v + x[i];
    else {
        int k = (i+j)/2;
        #pragma omp task
        scan_down(v, x, t, y, i, k);
        scan_down(v + t[k], x, t, y, k+1, j);
        #pragma omp taskwait
    }
}

template <typename T>
void printvec(T* x, int n) {
    cout << "[ ";
    for (int i = 0; i < n; i++) {
        cout << x[i] << " ";
    }
    cout << " ]" << endl;
}

