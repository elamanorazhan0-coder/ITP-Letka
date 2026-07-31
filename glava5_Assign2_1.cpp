// Exercises 5.1, 5.2, 5.3, 5.4, 5.5
#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

// 5.1  Bank account class
class BankAccount {
    char name[50];
    long accNumber;
    char accType[20];
    double balance;
public:
    void assign(const char n[], long accNo, const char type[], double bal) {
        strcpy(name, n);
        accNumber = accNo;
        strcpy(accType, type);
        balance = bal;
    }
    void deposit(double amount) {
        balance += amount;
    }
    void withdraw(double amount) {
        if (amount > balance)
            cout << "Insufficient balance for account " << accNumber << "!\n";
        else
            balance -= amount;
    }
    void display() const {
        cout << left << setw(15) << name
             << "Acc No: " << setw(10) << accNumber
             << "Type: " << setw(10) << accType
             << "Balance: " << fixed << setprecision(2) << balance << "\n";
    }
};

void demo5_1() {
    cout << "\n--- 5.1 Bank Account ---\n";
    BankAccount acc;
    acc.assign("Yelaman", 1001, "Savings", 5000.0);
    acc.deposit(1500.0);
    acc.withdraw(2000.0);
    acc.withdraw(10000.0);   // should fail - insufficient balance
    acc.display();
}

// 5.2  Vector class (series of float values)
class Vector {
    float *v;
    int n;
public:
    Vector() : v(nullptr), n(0) {}

    // deep copy constructor and assignment (needed since we pass
    // Vector objects by value into functions, e.g. addVectors())
    Vector(const Vector &other) : n(other.n) {
        v = new float[n];
        for (int i = 0; i < n; i++) v[i] = other.v[i];
    }
    Vector& operator=(const Vector &other) {
        if (this != &other) {
            delete[] v;
            n = other.n;
            v = new float[n];
            for (int i = 0; i < n; i++) v[i] = other.v[i];
        }
        return *this;
    }

    void create(int size) {
        n = size;
        v = new float[n];
        for (int i = 0; i < n; i++) v[i] = 0;
    }
    void modify(int index, float value) {
        if (index >= 0 && index < n) v[index] = value;
        else cout << "Index out of range!\n";
    }
    void multiply(float scalar) {
        for (int i = 0; i < n; i++) v[i] *= scalar;
    }
    void display() const {
        cout << "(";
        for (int i = 0; i < n; i++) {
            cout << v[i];
            if (i != n - 1) cout << ", ";
        }
        cout << ")\n";
    }
    // needed for 5.4
    int size() const { return n; }
    float get(int index) const { return (index >= 0 && index < n) ? v[index] : 0.f; }

    ~Vector() { delete[] v; }
};

void demo5_2() {
    cout << "\n--- 5.2 Vector class ---\n";
    Vector vec;
    vec.create(4);
    vec.modify(0, 10);
    vec.modify(1, 20);
    vec.modify(2, 30);
    vec.modify(3, 40);
    cout << "Vector: "; vec.display();
    vec.multiply(2);
    cout << "After multiplying by 2: "; vec.display();
}

// 5.3  Handling 10 customers
void demo5_3() {
    cout << "\n--- 5.3 Handling 10 customers ---\n";
    const int N = 10;
    BankAccount customers[N];
    for (int i = 0; i < N; i++) {
        char name[50];
        sprintf(name, "Customer%d", i + 1);
        customers[i].assign(name, 2000 + i, "Savings", 1000.0 * (i + 1));
    }
    customers[3].deposit(500);
    customers[7].withdraw(200);

    cout << "All 10 customer accounts:\n";
    for (int i = 0; i < N; i++)
        customers[i].display();
}

// 5.4  Modified Vector class: add two vectors
Vector addVectors(Vector a, Vector b) {   // objects passed as arguments
    Vector result;
    int n = (a.size() < b.size()) ? a.size() : b.size();
    result.create(n);
    for (int i = 0; i < n; i++)
        result.modify(i, a.get(i) + b.get(i));
    return result;
}

void demo5_4() {
    cout << "\n--- 5.4 Adding two vectors ---\n";
    Vector v1, v2;
    v1.create(3);
    v1.modify(0, 10); v1.modify(1, 20); v1.modify(2, 30);

    v2.create(3);
    v2.modify(0, 1); v2.modify(1, 2); v2.modify(2, 3);

    cout << "Vector 1: "; v1.display();
    cout << "Vector 2: "; v2.display();

    Vector sum = addVectors(v1, v2);
    cout << "Sum:      "; sum.display();
}

// 5.5  DM (metres/centimetres) and DB classes

class DB;

class DM {
    int metres;
    float cms;
public:
    void getData(int m, float c) { metres = m; cms = c; }
    void display() const {
        cout << metres << " metres and " << cms << " cm\n";
    }
    friend DM addDM(DM d1, DB d2);   // result stays in metres/cm
    friend DB addDB(DM d1, DB d2);   // result stays in feet/inches
};

class DB {
    int feet;
    float inches;
public:
    void getData(int f, float i) { feet = f; inches = i; }
    void display() const {
        cout << feet << " feet and " << inches << " inches\n";
    }
    friend DM addDM(DM d1, DB d2);
    friend DB addDB(DM d1, DB d2);
};

DM addDM(DM d1, DB d2) {
    float d1_total_cm = d1.metres * 100 + d1.cms;
    float d2_total_cm = (d2.feet * 12 + d2.inches) * 2.54f;
    float total_cm = d1_total_cm + d2_total_cm;

    DM result;
    result.metres = (int)(total_cm / 100);
    result.cms = total_cm - result.metres * 100;
    return result;
}

DB addDB(DM d1, DB d2) {
    float d1_total_in = (d1.metres * 100 + d1.cms) / 2.54f;
    float d2_total_in = d2.feet * 12 + d2.inches;
    float total_in = d1_total_in + d2_total_in;

    DB result;
    result.feet = (int)(total_in / 12);
    result.inches = total_in - result.feet * 12;
    return result;
}

void demo5_5() {
    cout << "\n--- 5.5 DM + DB using friend functions ---\n";
    DM d1;
    DB d2;
    d1.getData(2, 40.0f);   // 2 m 40 cm
    d2.getData(3, 6.0f);    // 3 ft 6 in

    cout << "DM object: "; d1.display();
    cout << "DB object: "; d2.display();

    DM sumInMetres = addDM(d1, d2);
    cout << "Sum in metres/cm: "; sumInMetres.display();

    DB sumInFeet = addDB(d1, d2);
    cout << "Sum in feet/inches: "; sumInFeet.display();
}

// ---------------------------------------------------------
int main() {
    demo5_1();
    demo5_2();
    demo5_3();
    demo5_4();
    demo5_5();
    return 0;
}