#include <iostream>
#include <cassert>
#include <omp.h>
#include <cstdlib>
#include <numeric>

using namespace std;

double* scan3(double*, int, int);
double* scan1(double*, int);
void scan_up(double*, double*, int, int, double& sum, int);
void scan_down(
        double, 
        double*, 
        double*, 
        double*, 
        int, 
        int, 
		int);
template <typename T>
void printvec(T*, int);

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "Usage: scan seed length n_threads bc" << endl;
        return -1;
    } 

    srand(atoi(argv[1])); 
    int length = atoi(argv[2]);
    int n_threads = atoi(argv[3]);
	int bc = atoi(argv[4]);
    omp_set_num_threads(n_threads); 
    
    double* vec = new double[length];
    for (int i = 0; i < length; i++) 
        vec[i] = rand() % 100;

	// run my implementation of scan on the array
    double start = omp_get_wtime();
    double* scanned3 = scan3(vec, length, bc);
    double pscan_time = omp_get_wtime() - start;
   
	// run naive sequential and STL sequential
    double* scanned1 = scan1(vec, length);
	double* incscanned = new double[length];
	start = omp_get_wtime();
	inclusive_scan(vec, vec+length, incscanned);
	double scan_time = omp_get_wtime() - start;

	//printvec(vec, length);
	//printvec(scanned3, length);
	//printvec(scanned1, length);

	// print array anytime that the two implementations don't match
    for (int i = 0; i < length; i++) {
        if (scanned3[i] != scanned1[i]) {
			cout << "Mismatched Results at index: " << i << endl;
			cout << "My Scan: from " << i-5 << " to " << i+5 << ": ";
			printvec(scanned3+i-5, 10);
			cout << "IncScan: from " << i-5 << " to " << i+5 << ": ";
			printvec(scanned1+i-5, 10);
			break;
		}
    }

	// output data in csv format
	cout << n_threads << ','
		 << length << ','
		 << bc << ','
		 << pscan_time << ','
	     << scan_time << endl;
}

double* scan3(double* x, int n, int bc) {
    double* y = new double[n];
    double* t = new double[n];

    y[0] = x[0];
   	
	if (bc >= n) {
		inclusive_scan(x, x+n, y);
		return y;
	}

	double sum;
    if (n > 1) {
        #pragma omp parallel
        #pragma omp single
        {
            scan_up(x, t, 1, n, sum, bc);
            scan_down(x[0], x, t, y, 1, n, bc);
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

// same as scan_up in sequential recursive code, but sum is modified
// as a passed-by-reference variable rather than as a return value
// from the function. Also performs an accumulate of the current level
// of the recursion tree when we reach our base case
void scan_up(double* x, double* t, int i, int j, double& sum, int bc) {
	if (i == j) {
		sum = x[i];
	} else if ((j-i) <= bc ) {
        sum = accumulate(x+i, x+j+1, 0);
	} else {
        int k = (i+j)/2;
        #pragma omp task
        scan_up(x,t,i,k, t[k], bc);
        double right;
        scan_up(x,t,k+1,j, right, bc);
        #pragma omp taskwait
        sum = t[k] + right;
    }
}

// same as scan_down in sequential recursive implementation but modify y 
// in-place and considers the base case. When our subarray is less than or
// equal to the base case size, we perform a sequential scan on the entire
// subarray calculate y at those indices
void scan_down(double v, 
        double* x, 
        double* t, 
        double* y, 
        int i, 
		int j,
		int bc) {
	if (i == j) {
		y[i] = v + x[i];
	} else if ((j - i) <= bc) {
		inclusive_scan(x+i, x+j+1, y+i, plus<>(), v);
	} else {
        int k = (i+j)/2;
        #pragma omp task
        scan_down(v, x, t, y, i, k, bc);
        scan_down(v + t[k], x, t, y, k+1, j, bc);
        #pragma omp taskwait
    }
}

template <typename T>
void printvec(T* x, int n) {
    cout << "[ ";
    for (int i = 0; i < n; i++) {
        cout << x[i] << " ";
    }
    cout << "]" << endl;
}

