// Exercises 6.1, 6.2, 6.3, 6.4, 6.5
#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

// 6.1  Constructors for classes designed in Exercises 5.1-5.5
namespace Ex6_1 {

//5.1 Bank account, now with a constructor
class BankAccount {
    char name[50];
    long accNumber;
    char accType[20];
    double balance;
public:
    BankAccount(const char n[] = "Unnamed", long accNo = 0,
                const char type[] = "Savings", double bal = 0.0) {
        strcpy(name, n);
        accNumber = accNo;
        strcpy(accType, type);
        balance = bal;
    }
    void deposit(double amount) { balance += amount; }
    void withdraw(double amount) {
        if (amount > balance) cout << "Insufficient balance!\n";
        else balance -= amount;
    }
    void display() const {
        cout << name << " | Acc " << accNumber << " | " << accType
             << " | Balance " << fixed << setprecision(2) << balance << "\n";
    }
};

//5.2 Vector, now with constructors
class Vector {
    float *v;
    int n;
public:
    Vector(int size = 0) : n(size) {
        v = (n > 0) ? new float[n]() : nullptr;
    }
    Vector(const Vector &o) : n(o.n) {
        v = new float[n];
        for (int i = 0; i < n; i++) v[i] = o.v[i];
    }
    Vector& operator=(const Vector &o) {
        if (this != &o) {
            delete[] v;
            n = o.n;
            v = new float[n];
            for (int i = 0; i < n; i++) v[i] = o.v[i];
        }
        return *this;
    }
    void modify(int i, float val) { if (i >= 0 && i < n) v[i] = val; }
    void multiply(float s) { for (int i = 0; i < n; i++) v[i] *= s; }
    void display() const {
        cout << "(";
        for (int i = 0; i < n; i++) cout << v[i] << (i != n - 1 ? ", " : "");
        cout << ")\n";
    }
    ~Vector() { delete[] v; }
};

//5.5 DM / DB, now with constructors
class DB;
class DM {
    int metres; float cms;
public:
    DM(int m = 0, float c = 0.0f) : metres(m), cms(c) {}
    void display() const { cout << metres << "m " << cms << "cm\n"; }
    friend DM addDM(DM d1, DB d2);
};
class DB {
    int feet; float inches;
public:
    DB(int f = 0, float i = 0.0f) : feet(f), inches(i) {}
    void display() const { cout << feet << "ft " << inches << "in\n"; }
    friend DM addDM(DM d1, DB d2);
};
DM addDM(DM d1, DB d2) {
    float total_cm = (d1.metres * 100 + d1.cms) + (d2.feet * 12 + d2.inches) * 2.54f;
    return DM((int)(total_cm / 100), total_cm - (int)(total_cm / 100) * 100);
}

void demo() {
    cout << "\n--- 6.1 Constructors for Chapter 5 classes ---\n";
    BankAccount acc("Yelaman", 1001, "Savings", 5000);
    acc.deposit(500);
    acc.display();

    Vector v1(3);
    v1.modify(0, 10); v1.modify(1, 20); v1.modify(2, 30);
    v1.display();

    DM d1(2, 40.0f);
    DB d2(3, 6.0f);
    DM sum = addDM(d1, d2);
    cout << "DM+DB sum: "; sum.display();
}

}

// 6.2  User-defined String class
namespace Ex6_2 {

class String {
    char *str;
    int len;
public:
    // uninitialized string (length 0)
    String() : len(0) {
        str = new char[1];
        str[0] = '\0';
    }
    // construct from a string constant
    String(const char *s) {
        len = strlen(s);
        str = new char[len + 1];
        strcpy(str, s);
    }
    // copy constructor (needed for s2 = s1 semantics & pass-by-value)
    String(const String &o) : len(o.len) {
        str = new char[len + 1];
        strcpy(str, o.str);
    }
    // assignment operator: makes "s2 = s1;" work correctly
    String& operator=(const String &o) {
        if (this != &o) {
            delete[] str;
            len = o.len;
            str = new char[len + 1];
            strcpy(str, o.str);
        }
        return *this;
    }
    // add two strings to make a third string
    String operator+(const String &o) const {
        String result;
        delete[] result.str;
        result.len = len + o.len;
        result.str = new char[result.len + 1];
        strcpy(result.str, str);
        strcat(result.str, o.str);
        return result;
    }
    void display() const {
        cout << "\"" << str << "\" (length " << len << ")\n";
    }
    ~String() { delete[] str; }
};

void demo() {
    cout << "\n--- 6.2 String class ---\n";
    String s1;                       // (a) uninitialized string
    cout << "s1 (uninitialized): "; s1.display();

    String s2("Well done!");         // (b) init with string constant
    cout << "s2: "; s2.display();

    s2 = s1;                         // reasonable copy expression
    cout << "s2 after s2 = s1: "; s2.display();

    String s3("Hello, ");
    String s4("world!");
    String s5 = s3 + s4;             // (c) concatenation
    cout << "s3 + s4 = "; s5.display();   // (d) display
}

}

// 6.3 / 6.4 / 6.5  Bookshop inventory ("books" class)
namespace Ex6_3_4_5 {

class books {
    char *title;
    char *author;
    char *publisher;
    double price;
    int stock;

    static int successfulTransactions;
    static int unsuccessfulTransactions;

    // 6.4(a) private member function to update price
    void updatePrice(double newPrice) {
        price = newPrice;
    }

public:
    books(const char *t, const char *a, const char *p, double pr, int st) {
        title = new char[strlen(t) + 1];   strcpy(title, t);
        author = new char[strlen(a) + 1];  strcpy(author, a);
        publisher = new char[strlen(p) + 1]; strcpy(publisher, p);
        price = pr;
        stock = st;
    }
    ~books() {
        delete[] title;
        delete[] author;
        delete[] publisher;
    }

    bool matches(const char *t, const char *a) const {
        return strcmp(title, t) == 0 && strcmp(author, a) == 0;
    }

    void displayDetails() const {
        cout << "Title: " << title << " | Author: " << author
             << " | Publisher: " << publisher
             << " | Price: " << fixed << setprecision(2) << price
             << " | In stock: " << stock << "\n";
    }

    // 6.3 + 6.4(b)/(c): process a purchase, auto-update stock,
    // record successful/unsuccessful transactions
    void purchase(int copies) {
        if (copies <= stock) {
            double total = copies * price;
            stock -= copies;                       // (b) auto stock update
            successfulTransactions++;               // (c) static counter
            cout << "Total cost for " << copies << " cop(y/ies): "
                 << fixed << setprecision(2) << total << "\n";
            cout << "Remaining stock: " << stock << "\n";
        } else {
            unsuccessfulTransactions++;              // (c) static counter
            cout << "Required copies not in stock\n";
        }
    }

    void setPrice(double newPrice) { updatePrice(newPrice); }  // 6.4(a) public wrapper

    static void showStats() {
        cout << "Successful transactions: " << successfulTransactions
             << " | Unsuccessful transactions: " << unsuccessfulTransactions << "\n";
    }

    // getters used for pointer-based access in 6.5
    const char* getTitle() const { return title; }
    const char* getAuthor() const { return author; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
};

int books::successfulTransactions = 0;
int books::unsuccessfulTransactions = 0;

void searchAndBuy(books *inventory[], int n, const char *title, const char *author, int copiesWanted) {
    for (int i = 0; i < n; i++) {
        if (inventory[i]->matches(title, author)) {
            cout << "Book found:\n";
            inventory[i]->displayDetails();
            inventory[i]->purchase(copiesWanted);
            return;
        }
    }
    cout << "Book \"" << title << "\" by " << author << " is not available.\n";
}

void demo() {
    cout << "\n--- 6.3 / 6.4 / 6.5 Bookshop inventory ---\n";
    const int N = 3;
    books *inventory[N];
    inventory[0] = new books("The C++ Programming Language", "Bjarne Stroustrup", "Addison-Wesley", 55.0, 5);
    inventory[1] = new books("Effective C++", "Scott Meyers", "Addison-Wesley", 45.0, 2);
    inventory[2] = new books("Clean Code", "Robert Martin", "Prentice Hall", 40.0, 0);

    searchAndBuy(inventory, N, "Effective C++", "Scott Meyers", 1);   // success
    searchAndBuy(inventory, N, "Effective C++", "Scott Meyers", 5);   // not enough stock
    searchAndBuy(inventory, N, "Unknown Book", "Nobody", 1);          // not found

    // 6.4(a): update price using private member function via public wrapper
    inventory[0]->setPrice(60.0);
    cout << "Updated price: "; inventory[0]->displayDetails();

    // 6.4(c): show transaction statistics (static data members)
    books::showStats();

    // 6.5: access members through pointers explicitly
    cout << "\nAccessing members via pointers:\n";
    books *ptr = inventory[0];
    cout << "Title via pointer: " << ptr->getTitle() << "\n";
    cout << "Price via pointer: " << (*ptr).getPrice() << "\n";
    cout << "Stock via pointer: " << ptr->getStock() << "\n";

    for (int i = 0; i < N; i++) delete inventory[i];
}

}

int main() {
    Ex6_1::demo();
    Ex6_2::demo();
    Ex6_3_4_5::demo();
    return 0;
}