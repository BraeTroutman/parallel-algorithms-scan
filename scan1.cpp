#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

vector<double> scan3(vector<double>);
vector<double> scan1(vector<double>);
double scan_up(vector<double>, vector<double>&, int, int);
void scan_down(double, vector<double>, vector<double>, vector<double>&, int, int);
double add(double, double);

template <typename T>
void printvec(vector<T>);

int main(int argc, char* argv[]) {
	vector<double> vec = {10, 100, 15, 397, 123, 90};
    vector<double> scanned3 = scan3(vec);
    vector<double> scanned1 = scan1(vec);

    assert(scanned3==scanned1);

    printvec(vec);
    printvec(scanned3);
}

vector<double> scan3(vector<double> x) {
    int len = x.size();
    vector<double> y (len, 0);
    vector<double> t (len, 0);
    y[0] = x[0];

    if (len > 1) {
        scan_up(x, t, 1, len);
        scan_down(x[0], x, t, y, 1, len);
    }
    return y;
}

vector<double> scan1(vector<double> x) {
    int len = x.size();
    double sum = 0;
    vector<double> y (len, 0);

    for (int i = 0; i < x.size(); i++) {
        sum += x[i];
        y[i] = sum;
    }

    return y;
}

double scan_up(vector<double> x, vector<double>& t, int i, int j) {
    if (i == j)
        return x[i];
    else {
        int k = (i+j)/2;
        t[k] = scan_up(x,t,i,k);
        double right = scan_up(x,t,k+1,j);
        return t[k] + right;
    }
}

void scan_down(double v, vector<double> x, vector<double> t, vector<double>& y, int i, int j) {
    if (i == j)
        y[i] = v + x[i];
    else {
        int k = (i+j)/2;
        scan_down(v, x, t, y, i, k);
        scan_down(v + t[k], x, t, y, k+1, j);
    }
}

double add(double x, double y) { return x + y; }

template <typename T>
void printvec(vector<T> x) {
    cout << "[ ";
    for (int i = 0; i < x.size(); i++) {
        cout << x[i] << " ";
    }
    cout << " ]" << endl;
}

