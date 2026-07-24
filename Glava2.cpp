// 2.1
#include <iostream>
using namespace std;

int main() {
    cout << "Maths     = 90\n"
         << "Physics   = 77\n"
         << "Chemistry = 69\n";
    return 0;
}


// 2.2

int main() {
    double a, b;
    cout << "Enter two numbers: ";
    cin >> a >> b;

    if (a > b)
        cout << "Larger value = " << a << endl;
    else if (b > a)
        cout << "Larger value = " << b << endl;
    else
        cout << "Both numbers are equal = " << a << endl;

    return 0;
}



// 2.3

int main() {
    int n;
    cout << "Enter an integer value: ";
    cin >> n;

    for (int i = 1; i <= n; i++)
        cout << "WELL DONE" << endl;

    return 0;
}



// 2.4

int main() {
    double a, b, c, x;

    cout << "Enter values of a, b and c: ";
    cin >> a >> b >> c;

    x = a / b - c;

    cout << "x = a / b - c = " << x << endl;

    return 0;
}



// 2.5

int main() {
    double fahrenheit, celsius;

    cout << "Enter temperature in Fahrenheit: ";
    cin >> fahrenheit;

    celsius = (fahrenheit - 32.0) * 5.0 / 9.0;

    cout << "Temperature in Celsius = " << celsius << endl;

    return 0;
}



// 2.6

class temp {
private:
    double fahrenheit;
    double celsius;

public:
    void getData() {
        cout << "Enter temperature in Fahrenheit: ";
        cin >> fahrenheit;
    }

    void convert() {
        celsius = (fahrenheit - 32.0) * 5.0 / 9.0;
    }

    void display() {
        cout << "Temperature in Celsius = " << celsius << endl;
    }
};

int main() {
    temp t;
    t.getData();
    t.convert();
    t.display();
    return 0;
}