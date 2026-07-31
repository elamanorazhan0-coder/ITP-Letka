// Exercises 10.1, 10.2, 10.3

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;

// 10.1  Read a list (name, code, cost) and print a 3-column
// table: name/code left-justified, cost right-justified, decimal places (trailing zeros kept).
namespace Ex10_1 {

struct Item {
    string name;
    int code;
    double cost;
};

void demo() {
    cout << "\n--- 10.1 Item list (3-column table) ---\n";

    // Normally these would come from cin (cin >> n; then a loop
    // asking for name/code/cost). I hardcode a couple of items
    // here just so the demo runs without needing keyboard input.
    Item items[] = {
        {"Turbo C++", 1001, 250.95},
        {"C Primer",  905,  95.70}
    };
    int n = 2;

    // header
    cout << left << setw(15) << "NAME"
         << setw(10) << "CODE"
         << right << setw(10) << "COST" << "\n";
    cout << string(35, '-') << "\n";

    // rows - fixed/setprecision(2) keeps trailing zeros like 95.70
    for (int i = 0; i < n; i++) {
        cout << left << setw(15) << items[i].name
             << setw(10) << items[i].code
             << right << fixed << setprecision(2) << setw(10) << items[i].cost
             << "\n";
    }
}

} // namespace Ex10_1

// 10.2  Same table, but empty space is filled with hyphens instead of blanks.
namespace Ex10_2 {

struct Item {
    string name;
    int code;
    double cost;
};

void demo() {
    cout << "\n--- 10.2 Same table, hyphens instead of blank space ---\n";

    Item items[] = {
        {"Turbo C++", 1001, 250.95},
        {"C Primer",  905,  95.70}
    };
    int n = 2;

    // setfill('-') changes what character setw() pads with.
    // We switch it back to ' ' before the header so the header
    // itself still looks normal, then turn hyphens on for the data.
    cout << left << setw(15) << "NAME"
         << setw(10) << "CODE"
         << right << setw(10) << "COST" << "\n";
    cout << string(35, '-') << "\n";

    cout << setfill('-');
    for (int i = 0; i < n; i++) {
        cout << left << setw(15) << items[i].name
             << setw(10) << items[i].code
             << right << fixed << setprecision(2) << setw(10) << items[i].cost
             << "\n";
    }
    cout << setfill(' ');   // reset fill char back to default
}

} // namespace Ex10_2

// 10.3  Read text typed by the user, count lines / words /
//       characters, print the results in two columns.
namespace Ex10_3 {

void demo() {
    cout << "\n--- 10.3 Line / word / character counter ---\n";
    cout << "Type your text line by line. Enter a single line with just\n";
    cout << "END to stop.\n";

    int lines = 0, words = 0, chars = 0;
    string line;

    while (true) {
        // if input isn't a real terminal (e.g. piped from a file),
        // getline just fails at EOF, which also stops the loop.
        if (!getline(cin, line)) break;
        if (line == "END") break;

        lines++;
        chars += (int)line.size();   // not counting the newline itself

        // counting words: split on whitespace using a stringstream
        istringstream iss(line);
        string word;
        while (iss >> word) words++;
    }

    cout << "\nResults:\n";
    cout << left  << setw(20) << "Number of lines:"  << right << setw(6) << lines  << "\n";
    cout << left  << setw(20) << "Number of words:"  << right << setw(6) << words  << "\n";
    cout << left  << setw(20) << "Number of characters:" << right << setw(6) << chars << "\n";
}

} // namespace Ex10_3

int main() {
    Ex10_1::demo();
    Ex10_2::demo();
    Ex10_3::demo();
    return 0;
}