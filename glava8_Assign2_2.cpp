// Exercises 8.1, 8.2, 8.3, 8.4

#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

// 8.1  account -> cur_acct / sav_acct  NO constructors, use
//      member functions to initialize
// 8.2  same hierarchy, but WITH constructors
// We implement both as two separate namespaces.

namespace Ex8_1 {

class account {
protected:
    char name[50];
    long accNumber;
    char accType[20];
    double balance;
public:
    void init(const char n[], long accNo, const char type[], double bal) {
        strcpy(name, n);
        accNumber = accNo;
        strcpy(accType, type);
        balance = bal;
    }
    void deposit(double amount) { balance += amount; }
    void display() const {
        cout << name << " | Acc " << accNumber << " | " << accType
             << " | Balance: " << fixed << setprecision(2) << balance << "\n";
    }
};

class sav_acct : public account {
    float rate;   // interest rate, e.g. 0.05 for 5%
public:
    void initSavings(const char n[], long accNo, double bal, float interestRate) {
        init(n, accNo, "Savings", bal);
        rate = interestRate;
    }
    void computeInterest() {
        double interest = balance * rate;
        balance += interest;
        cout << "Interest of " << fixed << setprecision(2) << interest << " credited.\n";
    }
    void withdraw(double amount) {
        if (amount > balance) cout << "Insufficient balance!\n";
        else balance -= amount;
    }
};

class cur_acct : public account {
    double minBalance;
    double serviceCharge;
public:
    void initCurrent(const char n[], long accNo, double bal, double minBal, double charge) {
        init(n, accNo, "Current", bal);
        minBalance = minBal;
        serviceCharge = charge;
    }
    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance!\n";
            return;
        }
        balance -= amount;
        if (balance < minBalance) {
            balance -= serviceCharge;
            cout << "Balance below minimum. Service charge of "
                 << serviceCharge << " applied.\n";
        }
    }
};

void demo() {
    cout << "\n--- 8.1 account/sav_acct/cur_acct (no constructors) ---\n";
    sav_acct s;
    s.initSavings("Yelaman", 3001, 10000, 0.05f);
    s.computeInterest();
    s.withdraw(5000);
    s.display();

    cur_acct c;
    c.initCurrent("Aigerim", 4001, 2000, 1000, 50);
    c.withdraw(1500);   // drops below min balance -> service charge
    c.display();
}

} // namespace Ex8_1

namespace Ex8_2 {

class account {
protected:
    char name[50];
    long accNumber;
    char accType[20];
    double balance;
public:
    account(const char n[] = "", long accNo = 0, const char type[] = "", double bal = 0.0)
        : accNumber(accNo), balance(bal) {
        strcpy(name, n);
        strcpy(accType, type);
    }
    void deposit(double amount) { balance += amount; }
    void display() const {
        cout << name << " | Acc " << accNumber << " | " << accType
             << " | Balance: " << fixed << setprecision(2) << balance << "\n";
    }
};

class sav_acct : public account {
    float rate;
public:
    sav_acct(const char n[], long accNo, double bal, float interestRate)
        : account(n, accNo, "Savings", bal), rate(interestRate) {}

    void computeInterest() {
        double interest = balance * rate;
        balance += interest;
        cout << "Interest of " << fixed << setprecision(2) << interest << " credited.\n";
    }
    void withdraw(double amount) {
        if (amount > balance) cout << "Insufficient balance!\n";
        else balance -= amount;
    }
};

class cur_acct : public account {
    double minBalance;
    double serviceCharge;
public:
    cur_acct(const char n[], long accNo, double bal, double minBal, double charge)
        : account(n, accNo, "Current", bal), minBalance(minBal), serviceCharge(charge) {}

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance!\n";
            return;
        }
        balance -= amount;
        if (balance < minBalance) {
            balance -= serviceCharge;
            cout << "Balance below minimum. Service charge of "
                 << serviceCharge << " applied.\n";
        }
    }
};

void demo() {
    cout << "\n--- 8.2 account/sav_acct/cur_acct (with constructors) ---\n";
    sav_acct s("Yelaman", 3001, 10000, 0.05f);
    s.computeInterest();
    s.withdraw(5000);
    s.display();

    cur_acct c("Aigerim", 4001, 2000, 1000, 50);
    c.withdraw(1500);
    c.display();
}

} // namespace Ex8_2

// 8.3  Staff database hierarchy (Fig. 8.14):
//      staff -> teacher, typist, officer
//      typist -> regular, casual
// 8.4  Add "education" class, inherited by teacher & officer

namespace Ex8_3_4 {

class staff {
protected:
    int code;
    char name[50];
public:
    staff(int c = 0, const char n[] = "") : code(c) { strcpy(name, n); }
    void display() const {
        cout << "Code: " << code << " | Name: " << name;
    }
};

// 8.4: education info, to be inherited by teacher and officer
class education {
protected:
    char generalQual[50];   // highest general education qualification
    char profQual[50];      // highest professional qualification
public:
    education(const char g[] = "", const char p[] = "") {
        strcpy(generalQual, g);
        strcpy(profQual, p);
    }
    void displayEducation() const {
        cout << " | General: " << generalQual << " | Professional: " << profQual;
    }
};

class teacher : public staff, public education {
    char subject[30];
    int publications;
public:
    teacher(int c, const char n[], const char subj[], int pubs,
            const char g[] = "", const char p[] = "")
        : staff(c, n), education(g, p), publications(pubs) {
        strcpy(subject, subj);
    }
    void display() const {
        staff::display();
        cout << " | Subject: " << subject << " | Publications: " << publications;
        displayEducation();
        cout << "\n";
    }
};

class officer : public staff, public education {
    int grade;
public:
    officer(int c, const char n[], int g, const char gen[] = "", const char p[] = "")
        : staff(c, n), education(gen, p), grade(g) {}
    void display() const {
        staff::display();
        cout << " | Grade: " << grade;
        displayEducation();
        cout << "\n";
    }
};

class typist : public staff {
protected:
    int speed;   // words per minute
public:
    typist(int c = 0, const char n[] = "", int sp = 0) : staff(c, n), speed(sp) {}
    void display() const {
        staff::display();
        cout << " | Speed: " << speed << " wpm";
    }
};

class regular : public typist {
    double salary;
public:
    regular(int c, const char n[], int sp, double sal)
        : typist(c, n, sp), salary(sal) {}
    void display() const {
        typist::display();
        cout << " | Salary: " << fixed << setprecision(2) << salary << "\n";
    }
};

class casual : public typist {
    double dailyWages;
public:
    casual(int c, const char n[], int sp, double wages)
        : typist(c, n, sp), dailyWages(wages) {}
    void display() const {
        typist::display();
        cout << " | Daily wages: " << fixed << setprecision(2) << dailyWages << "\n";
    }
};

void demo() {
    cout << "\n--- 8.3 / 8.4 Staff database hierarchy ---\n";
    teacher t(101, "Zhanna", "Mathematics", 5, "MSc", "PhD in Education");
    officer o(102, "Bekzat", 7, "BSc", "MBA");
    regular r(103, "Aliya", 60, 150000);
    casual cs(104, "Nurlan", 45, 5000);

    t.display();
    o.display();
    r.display();
    cs.display();
}

}

int main() {
    Ex8_1::demo();
    Ex8_2::demo();
    Ex8_3_4::demo();
    return 0;
}