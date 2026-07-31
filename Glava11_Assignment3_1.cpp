// Exercises 11.1, 11.2, 11.3, 11.4
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
using namespace std;

// 11.1  Copy a text file, collapsing runs of spaces into one.
namespace Ex11_1 {

void collapseSpaces(const string &inFile, const string &outFile) {
    ifstream in(inFile);
    ofstream out(outFile);

    if (!in) { cout << "Could not open " << inFile << "\n"; return; }

    char ch;
    bool lastWasSpace = false;   // tracks whether we just wrote a space
    while (in.get(ch)) {
        if (ch == ' ') {
            if (!lastWasSpace) out.put(' ');   // only write the first space of a run
            lastWasSpace = true;
        } else {
            out.put(ch);
            lastWasSpace = false;   // reset once we hit a non-space char (also resets on '\n')
        }
    }
}

void demo() {
    cout << "\n--- 11.1 Collapse multiple spaces into one ---\n";

    // making a sample input file so the demo is self-contained
    ofstream sample("input11_1.txt");
    sample << "This    is   a   test     file.\nSecond   line   here.\n";
    sample.close();

    collapseSpaces("input11_1.txt", "output11_1.txt");

    cout << "Result (output11_1.txt):\n";
    ifstream result("output11_1.txt");
    string line;
    while (getline(result, line)) cout << line << "\n";
}

} // namespace Ex11_1

// 11.2  Read "name number" pairs from a file, print as a two-column table (name left, number right).

namespace Ex11_2 {

void demo() {
    cout << "\n--- 11.2 Read phone list from file, print two columns ---\n";

    // creating the sample file mentioned in the exercise
    ofstream sample("phones11_2.txt");
    sample << "John 23456\nAhmed 9876\nZarina 771234\n";
    sample.close();

    ifstream in("phones11_2.txt");
    if (!in) { cout << "Could not open file\n"; return; }

    string name;
    long number;
    while (in >> name >> number) {   // whitespace-separated, exactly what >> is for
        cout << left << setw(15) << name << right << setw(10) << number << "\n";
    }
}

} // namespace Ex11_2

// ===========================================================
// 11.3  Store the same data using a class object, save to a
//binary data file (fixed-size records so we can jump straight to any record later - needed for Exercise 11.4).

namespace Ex11_3_4 {

class Contact {
    char name[30];
    char phone[15];
public:
    Contact() { name[0] = '\0'; phone[0] = '\0'; }
    Contact(const char n[], const char p[]) {
        strncpy(name, n, 29);  name[29] = '\0';
        strncpy(phone, p, 14); phone[14] = '\0';
    }
    const char* getName() const { return name; }
    const char* getPhone() const { return phone; }
    void setPhone(const char p[]) { strncpy(phone, p, 14); phone[14] = '\0'; }

    // used to search records - case-sensitive, matches the whole word
    bool nameIs(const string &n) const { return n == name; }
    bool phoneIs(const string &p) const { return p == phone; }
};

const char *DATA_FILE = "contacts11_3.dat";

void createDataFile() {
    // fixed-size binary records -> each Contact takes the same
    // number of bytes, so record i always starts at i * sizeof(Contact)
    ofstream out(DATA_FILE, ios::binary | ios::trunc);

    Contact list[] = {
        Contact("John", "23456"),
        Contact("Ahmed", "9876"),
        Contact("Zarina", "771234")
    };
    for (auto &c : list)
        out.write(reinterpret_cast<char*>(&c), sizeof(Contact));

    out.close();
}

void demo11_3() {
    cout << "\n--- 11.3 Save phone list as binary Contact records ---\n";
    createDataFile();
    cout << "Data file " << DATA_FILE << " created with 3 records.\n";
}

// 11.4  Menu-driven program working on the file from 11.3

// (a) find phone number given a name
void findPhoneByName() {
    string name;
    cout << "Enter name: ";
    cin >> name;

    fstream file(DATA_FILE, ios::binary | ios::in);
    Contact c;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&c), sizeof(Contact))) {
        if (c.nameIs(name)) {
            cout << name << "'s number is " << c.getPhone() << "\n";
            found = true;
            break;
        }
    }
    if (!found) cout << "Name not found.\n";
}

// (b) find name given a phone number
void findNameByPhone() {
    string phone;
    cout << "Enter phone number: ";
    cin >> phone;

    fstream file(DATA_FILE, ios::binary | ios::in);
    Contact c;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&c), sizeof(Contact))) {
        if (c.phoneIs(phone)) {
            cout << phone << " belongs to " << c.getName() << "\n";
            found = true;
            break;
        }
    }
    if (!found) cout << "Number not found.\n";
}

// (c) update a phone number in place, no need to rewrite whole file
void updatePhone() {
    string name, newPhone;
    cout << "Enter name to update: ";
    cin >> name;
    cout << "Enter new phone number: ";
    cin >> newPhone;

    // open for both read and write - that's what lets us seek back and overwrite just the one record we found
    fstream file(DATA_FILE, ios::binary | ios::in | ios::out);
    Contact c;
    int index = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&c), sizeof(Contact))) {
        if (c.nameIs(name)) {
            found = true;
            break;
        }
        index++;
    }

    if (found) {
        c.setPhone(newPhone.c_str());
        file.clear();                               // clear eof flag from the read above
        file.seekp(index * sizeof(Contact));        // jump back to that record's position
        file.write(reinterpret_cast<char*>(&c), sizeof(Contact));
        cout << "Updated.\n";
    } else {
        cout << "Name not found, nothing updated.\n";
    }
}

void printAll() {
    fstream file(DATA_FILE, ios::binary | ios::in);
    Contact c;
    cout << left << setw(15) << "NAME" << right << setw(10) << "PHONE" << "\n";
    while (file.read(reinterpret_cast<char*>(&c), sizeof(Contact)))
        cout << left << setw(15) << c.getName() << right << setw(10) << c.getPhone() << "\n";
}

void demo11_4() {
    cout << "\n--- 11.4 Menu-driven access to the contact file ---\n";
    cout << "(Running a couple of operations automatically for the demo;\n";
    cout << " in real use this would be an interactive cin >> choice loop.)\n\n";

    cout << "Current records:\n";
    printAll();

    // simulate: menu choice (a) look up Ahmed's number
    cout << "\n> Simulated menu choice: find phone by name (Ahmed)\n";
    {
        fstream file(DATA_FILE, ios::binary | ios::in);
        Contact c;
        while (file.read(reinterpret_cast<char*>(&c), sizeof(Contact))) {
            if (c.nameIs("Ahmed")) { cout << "Ahmed's number is " << c.getPhone() << "\n"; break; }
        }
    }

    // simulate: menu choice (c) update Zarina's number
    cout << "\n> Simulated menu choice: update phone (Zarina -> 700000)\n";
    {
        fstream file(DATA_FILE, ios::binary | ios::in | ios::out);
        Contact c;
        int index = 0;
        while (file.read(reinterpret_cast<char*>(&c), sizeof(Contact))) {
            if (c.nameIs("Zarina")) {
                c.setPhone("700000");
                file.clear();
                file.seekp(index * sizeof(Contact));
                file.write(reinterpret_cast<char*>(&c), sizeof(Contact));
                break;
            }
            index++;
        }
    }

    cout << "\nRecords after update:\n";
    printAll();

    /*Real interactive version would look like this:

    int choice;
    do {
        cout << "\n1. Find phone by name\n2. Find name by phone\n"
             << "3. Update phone\n4. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: findPhoneByName(); break;
            case 2: findNameByPhone(); break;
            case 3: updatePhone();     break;
        }
    } while (choice != 4);
    */
}

} // namespace Ex11_3_4

int main() {
    Ex11_1::demo();
    Ex11_2::demo();
    Ex11_3_4::demo11_3();
    Ex11_3_4::demo11_4();
    return 0;
}