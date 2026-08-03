// Exercises 14.1 - 14.8
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <string>
using namespace std;

// 14.1  Basic vector operations (code segment)
namespace Ex14_1 {

void demo() {
    cout << "\n--- 14.1 Basic vector code segment ---\n";
    vector<int> v(10);      // (a) max size 10 (all zero-initialized)
    v[0] = 0;                // (b) first element
    v[9] = 9;                // (c) last element
    for (int i = 1; i < 9; i++) v[i] = 1;   // (d) everything else

    cout << "Contents of v: ";
    for (int x : v) cout << x << " ";   // (e) display
    cout << "\n";
}

} // namespace Ex14_1

// 14.2  find() algorithm
namespace Ex14_2 {

void demo() {
    cout << "\n--- 14.2 find() algorithm ---\n";
    vector<int> v = {10, 20, 30, 40, 50};

    int target = 30;
    auto it = find(v.begin(), v.end(), target);

    if (it != v.end())
        cout << target << " found at position " << (it - v.begin()) << "\n";
    else
        cout << target << " not found\n";
}

} // namespace Ex14_2

// 14.3  count() algorithm
namespace Ex14_3 {

void demo() {
    cout << "\n--- 14.3 count() algorithm ---\n";
    vector<int> v = {5, 3, 5, 7, 5, 9, 3};

    int target = 5;
    int occurrences = count(v.begin(), v.end(), target);
    cout << target << " appears " << occurrences << " times\n";
}

} // namespace Ex14_3

// 14.4  merge() two sorted sequences into a vector
namespace Ex14_4 {

void demo() {
    cout << "\n--- 14.4 merge() two sequences ---\n";
    int evens[] = {2, 4, 6, 8, 10};             // array with even numbers
    list<int> odds = {1, 3, 5, 7, 9};           // list with odd numbers

    // merge() needs both inputs sorted (which they already are here)
    // and a destination big enough to hold both - hence the vector
    // is pre-sized to evens+odds and we merge into its begin().
    vector<int> result(5 + odds.size());
    merge(evens, evens + 5, odds.begin(), odds.end(), result.begin());

    cout << "Merged vector: ";
    for (int x : result) cout << x << " ";
    cout << "\n";
}

} // namespace Ex14_4

// 14.5  student class stored in a list, lookup both ways
namespace Ex14_5 {

class student {
    string name;
    int roll_number;
public:
    student(string n = "", int r = 0) : name(n), roll_number(r) {}
    string getName() const { return name; }
    int getRoll() const { return roll_number; }
};

void demo() {
    cout << "\n--- 14.5 student list, lookup by name/roll_number ---\n";
    list<student> phone_lit = {
        student("Yelaman", 101),
        student("Aigerim", 102),
        student("Bekzat", 103),
        student("Zarina", 104),
        student("Nurlan", 105)
    };

    // lookup 1: given a roll number, find the name
    int searchRoll = 103;
    for (const auto &s : phone_lit) {
        if (s.getRoll() == searchRoll) {
            cout << "Roll " << searchRoll << " -> " << s.getName() << "\n";
            break;
        }
    }

    // lookup 2: given a name, find the roll number
    string searchName = "Zarina";
    for (const auto &s : phone_lit) {
        if (s.getName() == searchName) {
            cout << searchName << " -> Roll " << s.getRoll() << "\n";
            break;
        }
    }
}

} // namespace Ex14_5

// 14.6  Same idea as 14.5, but using a set instead of a list
//       (the book's Exercise 14.17 isn't in our scan, so this
//       just redoes 14.5's student lookup with a std::set,
//       ordered by roll_number so lookups can use set's own find() instead of a manual loop)
namespace Ex14_6 {

class student {
    string name;
    int roll_number;
public:
    student(string n = "", int r = 0) : name(n), roll_number(r) {}
    string getName() const { return name; }
    int getRoll() const { return roll_number; }
    // sets need a way to order elements - order by roll_number
    bool operator<(const student &other) const {
        return roll_number < other.roll_number;
    }
};

void demo() {
    cout << "\n--- 14.6 student set (redo of 14.5 using set) ---\n";
    set<student> students = {
        student("Yelaman", 101),
        student("Aigerim", 102),
        student("Bekzat", 103)
    };

    // set::find() needs an object to compare against - we only
    // know the roll number, so build a dummy student with it
    int searchRoll = 102;
    auto it = students.find(student("", searchRoll));
    if (it != students.end())
        cout << "Roll " << searchRoll << " -> " << it->getName() << "\n";
    else
        cout << "Roll not found\n";
}

} // namespace Ex14_6

// 14.7  Car models & units sold, compute total value of a model given its unit cost
namespace Ex14_7 {

void demo() {
    cout << "\n--- 14.7 Car models: units sold, compute total value ---\n";

    // map<model, units sold> - perfect fit since we look things
    // up by model name
    map<string, int> unitsSold = {
        {"Toyota Camry", 120},
        {"Honda Civic", 95},
        {"Hyundai Elantra", 80}
    };

    string model = "Honda Civic";
    double unitCost = 15000.0;   // normally read interactively via cin

    auto it = unitsSold.find(model);
    if (it != unitsSold.end()) {
        double total = it->second * unitCost;
        cout << fixed << setprecision(2);
        cout << model << ": " << it->second << " units x " << unitCost
             << " = " << total << "\n";
    } else {
        cout << "Model not found\n";
    }
}

} // namespace Ex14_7

// 14.8  Shopping list of 5 items in a vector: delete, insert at position, add at end, print
namespace Ex14_8 {

void printList(const vector<string> &items) {
    cout << "Shopping list: ";
    for (const auto &item : items) cout << item << " | ";
    cout << "\n";
}

void demo() {
    cout << "\n--- 14.8 Shopping list operations ---\n";

    // normally read 5 items from cin in a loop; hardcoded here for the demo
    vector<string> items = {"Milk", "Bread", "Eggs", "Butter", "Cheese"};
    printList(items);

    // (a) delete a specified item
    auto it = find(items.begin(), items.end(), "Bread");
    if (it != items.end()) items.erase(it);
    cout << "After deleting \"Bread\":\n";
    printList(items);

    // (b) add an item at a specified location (index 1)
    items.insert(items.begin() + 1, "Juice");
    cout << "After inserting \"Juice\" at position 1:\n";
    printList(items);

    // (c) add an item at the end
    items.push_back("Apples");
    cout << "After adding \"Apples\" at the end:\n";
    printList(items);

    // (d) print contents - already have printList() doing that above
}

} // namespace Ex14_8

int main() {
    Ex14_1::demo();
    Ex14_2::demo();
    Ex14_3::demo();
    Ex14_4::demo();
    Ex14_5::demo();
    Ex14_6::demo();
    Ex14_7::demo();
    Ex14_8::demo();
    return 0;
}