// 4.1
#include <iostream>
using namespace std;

const int MAX_SIZE = 20;

void readMatrix(int mat[][MAX_SIZE], int m, int n) {
    cout << "Enter " << m << "x" << n << " matrix elements:\n";
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            cout << "Element [" << i << "][" << j << "]: ";
            cin >> mat[i][j];
        }
}

int main() {
    int m, n;
    int mat[MAX_SIZE][MAX_SIZE];

    cout << "Enter number of rows (m): ";
    cin >> m;
    cout << "Enter number of columns (n): ";
    cin >> n;

    readMatrix(mat, m, n);

    cout << "\nMatrix read successfully:\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            cout << mat[i][j] << " ";
        cout << endl;
    }

    return 0;
}



// 4.2

const int MAX_SIZE = 20;

void readMatrix(int mat[][MAX_SIZE], int m, int n) {
    cout << "Enter " << m << "x" << n << " matrix elements:\n";
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            cin >> mat[i][j];
}

void displayMatrix(int mat[][MAX_SIZE], int m, int n) {
    cout << "\nThe matrix is:\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            cout << mat[i][j] << "\t";
        cout << endl;
    }
}

int main() {
    int m, n;
    int mat[MAX_SIZE][MAX_SIZE];

    cout << "Enter number of rows (m): ";
    cin >> m;
    cout << "Enter number of columns (n): ";
    cin >> n;

    readMatrix(mat, m, n);
    displayMatrix(mat, m, n);

    return 0;
}



// 4.3
#include <iostream>
using namespace std;

const int MAX_SIZE = 20;
const int DEFAULT_ROWS = 3;   

void readMatrix(int mat[][MAX_SIZE], int n, int m = DEFAULT_ROWS) {
    cout << "Enter " << m << "x" << n << " matrix elements:\n";
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            cin >> mat[i][j];
}

void displayMatrix(int mat[][MAX_SIZE], int n, int m = DEFAULT_ROWS) {
    cout << "\nThe matrix is:\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            cout << mat[i][j] << "\t";
        cout << endl;
    }
}

int main() {
    int mat[MAX_SIZE][MAX_SIZE];
    int n, m;

    cout << "Enter number of columns (n): ";
    cin >> n;

    cout << "Do you want to use the default number of rows (" << DEFAULT_ROWS << ")? (1=yes, 0=no): ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        readMatrix(mat, n);          
        displayMatrix(mat, n);
    } else {
        cout << "Enter number of rows (m): ";
        cin >> m;
        readMatrix(mat, n, m);       
        displayMatrix(mat, n, m);
    }

    return 0;
}



// 4.4

double areaDefault(double length, double breadth = 1.0) {
    return length * breadth;
}

double areaOverload(double length, double breadth) {
    return length * breadth;
}
double areaOverload(double length) {  
    return areaOverload(length, 1.0);
}

int main() {
    cout << "Using default argument:\n";
    cout << "areaDefault(5)     = " << areaDefault(5) << endl;
    cout << "areaDefault(5, 3)  = " << areaDefault(5, 3) << endl;

    cout << "\nUsing function overloading:\n";
    cout << "areaOverload(5)    = " << areaOverload(5) << endl;
    cout << "areaOverload(5, 3) = " << areaOverload(5, 3) << endl;

    return 0;
}



// 4.5

#define LARGEST(a, b, c) ((a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c)))

int main() {
    double x, y, z;
    cout << "Enter three numbers: ";
    cin >> x >> y >> z;

    cout << "Largest = " << LARGEST(x, y, z) << endl;

    return 0;
}



// 4.6

inline double largest(double a, double b, double c) {
    double max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}

int main() {
    double x, y, z;
    cout << "Enter three numbers: ";
    cin >> x >> y >> z;

    cout << "Largest = " << largest(x, y, z) << endl;

    return 0;
}



// 4.7
#include <iostream>
using namespace std;

double power(double m, int n = 2) {
    double result = 1.0;
    for (int i = 0; i < n; i++)
        result *= m;
    return result;
}

int main() {
    double m;
    int n;

    cout << "Enter value of m: ";
    cin >> m;
    cout << "Enter value of n (enter -1 to use default n=2): ";
    cin >> n;

    if (n == -1)
        cout << m << " squared (default n=2) = " << power(m) << endl;
    else
        cout << m << " raised to power " << n << " = " << power(m, n) << endl;

    return 0;
}



// 4.8

double power(double m, int n = 2) {
    double result = 1.0;
    for (int i = 0; i < n; i++)
        result *= m;
    return result;
}

int power(int m, int n = 2) {
    int result = 1;
    for (int i = 0; i < n; i++)
        result *= m;
    return result;
}

int main() {
    double dm;
    int im, n;

    cout << "-- Testing power(double, int) --\n";
    cout << "Enter a double value for m: ";
    cin >> dm;
    cout << "Enter value of n (enter -1 to use default n=2): ";
    cin >> n;
    if (n == -1)
        cout << dm << " squared (default n=2) = " << power(dm) << endl;
    else
        cout << dm << " raised to power " << n << " = " << power(dm, n) << endl;

    cout << "\n-- Testing power(int, int) --\n";
    cout << "Enter an int value for m: ";
    cin >> im;
    cout << "Enter value of n (enter -1 to use default n=2): ";
    cin >> n;
    if (n == -1)
        cout << im << " squared (default n=2) = " << power(im) << endl;
    else
        cout << im << " raised to power " << n << " = " << power(im, n) << endl;

    return 0;
}