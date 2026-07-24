// 3.1
#include <iostream>
using namespace std;

void swapValues(int &x, int &y) {
    int temp = x;
    x = y;
    y = temp;
}

int main() {
    int a, b;
    cout << "Enter two integers: ";
    cin >> a >> b;

    cout << "Before swap: a = " << a << ", b = " << b << endl;
    swapValues(a, b);
    cout << "After swap:  a = " << a << ", b = " << b << endl;

    return 0;
}



// 3.2

int* createVector(int M) {
    int *v = new int[M];
    for (int i = 0; i < M; i++)
        v[i] = 0;        
    return v;
}

int main() {
    int M;
    cout << "Enter size of vector M: ";
    cin >> M;

    int *vec = createVector(M);

    cout << "Enter " << M << " values: ";
    for (int i = 0; i < M; i++)
        cin >> vec[i];

    cout << "Vector elements: ";
    for (int i = 0; i < M; i++)
        cout << vec[i] << " ";
    cout << endl;

    delete[] vec;
    return 0;
}



// 3.3

int main() {
    int n;
    cout << "Enter number of rows: ";
    cin >> n;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++)
            cout << i;
        cout << endl;
    }

    return 0;
}



// 3.4
#include <iomanip>

int main() {
    cout << fixed << setprecision(2);

    for (double P = 1000; P <= 10000; P += 1000) {
        for (double r = 0.10; r <= 0.20 + 1e-9; r += 0.01) {

            cout << "\nP = " << P << "   r = " << r << endl;
            cout << setw(6) << "n" << setw(15) << "V" << endl;

            double principal = P;   
            for (int n = 1; n <= 10; n++) {
                double V = principal * (1 + r);
                cout << setw(6) << n << setw(15) << V << endl;
                principal = V;   
            }
        }
    }

    return 0;
}



// 3.5

int main() {
    const int NUM_CANDIDATES = 5;
    int count[NUM_CANDIDATES] = {0}; 
    int spoiltBallots = 0;
    int totalBallots, vote;

    cout << "Enter total number of ballots: ";
    cin >> totalBallots;

    for (int i = 0; i < totalBallots; i++) {
        cout << "Enter candidate number for ballot " << (i + 1) << ": ";
        cin >> vote;

        if (vote >= 1 && vote <= NUM_CANDIDATES)
            count[vote - 1]++;
        else
            spoiltBallots++;
    }

    cout << "\n---- Election Results ----\n";
    for (int i = 0; i < NUM_CANDIDATES; i++)
        cout << "Candidate " << (i + 1) << ": " << count[i] << " votes\n";

    cout << "Spoilt ballots: " << spoiltBallots << endl;

    return 0;
}



// 3.6
#include <string>
using namespace std;

const int MAX_PLAYERS = 50;

int main() {
    string names[MAX_PLAYERS];
    int runs[MAX_PLAYERS];
    int innings[MAX_PLAYERS];
    int notOut[MAX_PLAYERS];
    double average[MAX_PLAYERS];

    int n;
    cout << "Enter number of players: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        cout << "\nPlayer " << (i + 1) << ":\n";
        cout << "  Name: ";
        cin >> names[i];
        cout << "  Runs: ";
        cin >> runs[i];
        cout << "  Innings: ";
        cin >> innings[i];
        cout << "  Times not out: ";
        cin >> notOut[i];
    }

    for (int i = 0; i < n; i++) {
        int timesOut = innings[i] - notOut[i];
        if (timesOut == 0) {
            average[i] = runs[i];
        } else {
            average[i] = static_cast<double>(runs[i]) / timesOut;
        }
    }

    cout << "\n\n";
    cout << left << setw(15) << "Player's name"
         << setw(10) << "Runs"
         << setw(12) << "Innings"
         << setw(18) << "Times not out"
         << setw(12) << "Average" << endl;

    cout << string(65, '-') << endl;

    for (int i = 0; i < n; i++) {
        cout << left << setw(15) << names[i]
             << setw(10) << runs[i]
             << setw(12) << innings[i]
             << setw(18) << notOut[i]
             << fixed << setprecision(2) << setw(12) << average[i] << endl;
    }

    return 0;
}



// 3.7
#include <cmath>
using namespace std;

double seriesSin(double x) {
    double term = x;
    double sum = x;
    int n = 1;

    while (fabs(term) > 1e-8 * fabs(sum) || n <= 1) {  // 0.0001% accuracy
        term = -term * x * x / ((2 * n) * (2 * n + 1));
        sum += term;
        n++;
        if (n > 100) break; // safety limit
    }
    return sum;
}

double seriesSum() {
    double sum = 1.0;
    int n = 2;
    double term;

    do {
        term = pow(1.0 / n, n);
        sum += term;
        n++;
    } while (term > 1e-8 * sum && n < 1000);

    return sum;
}

double seriesCos(double x) {
    double term = 1.0;
    double sum = 1.0;
    int n = 1;

    while (fabs(term) > 1e-8 * fabs(sum) || n <= 1) {
        term = -term * x * x / ((2 * n - 1) * (2 * n));
        sum += term;
        n++;
        if (n > 100) break;
    }
    return sum;
}

int main() {
    double x;
    cout << fixed << setprecision(6);

    cout << "Enter x (in radians) for sin(x) and cos(x): ";
    cin >> x;

    cout << "\nsin(" << x << ") [series] = " << seriesSin(x) << endl;
    cout << "sin(" << x << ") [library] = " << sin(x) << endl;

    cout << "\ncos(" << x << ") [series] = " << seriesCos(x) << endl;
    cout << "cos(" << x << ") [library] = " << cos(x) << endl;

    cout << "\nSUM = 1 + (1/2)^2 + (1/3)^3 + ... = " << seriesSum() << endl;

    return 0;
}



// 3.8
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    cout << "TABLE FOR Y = EXP(-X)\n\n";

    cout << setw(6) << "X";
    for (int col = 1; col <= 9; col++)
        cout << setw(10) << (col / 10.0);
    cout << endl;

    cout << fixed << setprecision(4);

    for (int row = 0; row <= 9; row++) {
        double xBase = row * 1.0;
        cout << setw(6) << xBase;

        for (int col = 1; col <= 9; col++) {
            double x = xBase + col / 10.0;
            if (x > 10.0) break;
            cout << setw(10) << exp(-x);
        }
        cout << endl;
    }

    return 0;
}



// 3.9
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main() {
    int N;
    cout << "Enter the number of values N: ";
    cin >> N;

    double *x = new double[N];
    double sum = 0.0;

    for (int i = 0; i < N; i++) {
        cout << "Enter x[" << i + 1 << "]: ";
        cin >> x[i];
        sum += x[i];
    }

    double mean = sum / N;

    double sumSqDiff = 0.0;
    for (int i = 0; i < N; i++)
        sumSqDiff += (x[i] - mean) * (x[i] - mean);

    double variance = sumSqDiff / N;
    double stdDeviation = sqrt(variance);

    cout << fixed << setprecision(4);
    cout << "\nMean               = " << mean << endl;
    cout << "Variance           = " << variance << endl;
    cout << "Standard Deviation = " << stdDeviation << endl;

    delete[] x;
    return 0;
}



// 3.10

double calculateCharge(int units) {
    double charge = 0.0;

    if (units <= 100) {
        charge = units * 0.60;
    } else if (units <= 300) {
        charge = 100 * 0.60 + (units - 100) * 0.80;
    } else {
        charge = 100 * 0.60 + 200 * 0.80 + (units - 300) * 0.90;
    }

    if (charge < 50.00)
        charge = 50.00;

    if (charge > 300.00)
        charge += charge * 0.15;   // 15% surcharge

    return charge;
}

int main() {
    int numUsers;
    cout << "Enter number of users: ";
    cin >> numUsers;

    string *names = new string[numUsers];
    int *units = new int[numUsers];
    double *charges = new double[numUsers];

    for (int i = 0; i < numUsers; i++) {
        cout << "\nEnter name of user " << (i + 1) << ": ";
        cin >> names[i];
        cout << "Enter units consumed: ";
        cin >> units[i];
        charges[i] = calculateCharge(units[i]);
    }

    cout << fixed << setprecision(2);
    cout << "\n" << left << setw(15) << "Name"
         << setw(10) << "Units"
         << setw(15) << "Charge (Rs.)" << endl;
    cout << string(40, '-') << endl;

    for (int i = 0; i < numUsers; i++) {
        cout << left << setw(15) << names[i]
             << setw(10) << units[i]
             << setw(15) << charges[i] << endl;
    }

    delete[] names;
    delete[] units;
    delete[] charges;

    return 0;
}