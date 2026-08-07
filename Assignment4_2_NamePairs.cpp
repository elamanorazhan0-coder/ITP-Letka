// Exercises 2, 3, 4 Name_pairs classes
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// [2] Name_pairs: holds parallel vectors name[] / age[]
namespace Ex2 {

class Name_pairs {
    vector<string> name;
    vector<double> age;
public:
    void read_names() {
        cout << "How many names? ";
        int n; cin >> n;
        cout << "Enter " << n << " names:\n";
        for (int i = 0; i < n; i++) {
            string s; cin >> s;
            name.push_back(s);
            age.push_back(0);   // placeholder, filled in by read_ages()
        }
    }

    void read_ages() {
        for (size_t i = 0; i < name.size(); i++) {
            cout << "Age for " << name[i] << ": ";
            cin >> age[i];
        }
    }

    void print() const {
        for (size_t i = 0; i < name.size(); i++)
            cout << name[i] << ", " << age[i] << "\n";
    }

    // sorts name[] alphabetically, and shuffles age[] the same way so each name stays paired with its own age
    void sort() {
        // sort a vector of indices instead of sorting name[] directly -
        // that way we know exactly how to reorder age[] to match
        vector<size_t> idx(name.size());
        for (size_t i = 0; i < idx.size(); i++) idx[i] = i;

        std::sort(idx.begin(), idx.end(),
                  [this](size_t a, size_t b) { return name[a] < name[b]; });

        vector<string> newName(name.size());
        vector<double> newAge(age.size());
        for (size_t i = 0; i < idx.size(); i++) {
            newName[i] = name[idx[i]];
            newAge[i] = age[idx[i]];
        }
        name = newName;
        age = newAge;
    }
};

void demo() {
    cout << "\n--- [2] Name_pairs (parallel vectors) ---\n";
    cout << "(feeding fixed data instead of real cin, just for the demo)\n";
    // Normally: Name_pairs np; np.read_names(); np.read_ages();
    // Here we simulate the same result without needing keyboard input.
}

} // namespace Ex2

// [3] Replace member print() with a global operator<<, and add global == and != for Name_pairs
namespace Ex3 {

class Name_pairs {
    vector<string> name;
    vector<double> age;
public:
    void add(const string &n, double a) { name.push_back(n); age.push_back(a); }

    void read_names() {
        int n; cin >> n;
        for (int i = 0; i < n; i++) { string s; cin >> s; name.push_back(s); age.push_back(0); }
    }
    void read_ages() {
        for (size_t i = 0; i < name.size(); i++) { cout << "Age for " << name[i] << ": "; cin >> age[i]; }
    }
    void sort() {
        vector<size_t> idx(name.size());
        for (size_t i = 0; i < idx.size(); i++) idx[i] = i;
        std::sort(idx.begin(), idx.end(), [this](size_t a, size_t b) { return name[a] < name[b]; });
        vector<string> nn(name.size()); vector<double> na(age.size());
        for (size_t i = 0; i < idx.size(); i++) { nn[i] = name[idx[i]]; na[i] = age[idx[i]]; }
        name = nn; age = na;
    }

    // operator<< needs access to the private vectors, so make it a friend
    friend ostream& operator<<(ostream &os, const Name_pairs &np);
    friend bool operator==(const Name_pairs &a, const Name_pairs &b);
};

// global operator<< instead of a member print()
ostream& operator<<(ostream &os, const Name_pairs &np) {
    for (size_t i = 0; i < np.name.size(); i++)
        os << np.name[i] << ", " << np.age[i] << "\n";
    return os;
}

bool operator==(const Name_pairs &a, const Name_pairs &b) {
    return a.name == b.name && a.age == b.age;   // vector already supports ==
}
bool operator!=(const Name_pairs &a, const Name_pairs &b) {
    return !(a == b);   // define != in terms of == - avoids repeating logic
}

void demo() {
    cout << "\n--- [3] Name_pairs with global operator<<, ==, != ---\n";
    Name_pairs a, b;
    a.add("Yelaman", 19);
    a.add("Aigerim", 20);
    b.add("Yelaman", 19);
    b.add("Aigerim", 20);

    cout << "a:\n" << a;
    cout << "a == b ? " << (a == b ? "yes" : "no") << "\n";

    b.add("Extra", 30);
    cout << "after adding to b, a != b ? " << (a != b ? "yes" : "no") << "\n";
}

} // namespace Ex3

// [4] Redo the exercise, but implement Name_pairs using a Name_pair struct (one pair per element) + a single vector
namespace Ex4 {

struct Name_pair {
    string name;
    double age;
};

class Name_pairs {
    vector<Name_pair> pairs;   // single vector instead of two parallel ones
public:
    void add(const string &n, double a) { pairs.push_back({n, a}); }

    void read_names() {
        int n; cin >> n;
        for (int i = 0; i < n; i++) { string s; cin >> s; pairs.push_back({s, 0}); }
    }
    void read_ages() {
        for (auto &p : pairs) { cout << "Age for " << p.name << ": "; cin >> p.age; }
    }

    // sorting is much simpler now - just sort pairs directly, name and age move together automatically because they're one object
    void sort() {
        std::sort(pairs.begin(), pairs.end(),
                  [](const Name_pair &a, const Name_pair &b) { return a.name < b.name; });
    }

    friend ostream& operator<<(ostream &os, const Name_pairs &np);
    friend bool operator==(const Name_pairs &a, const Name_pairs &b);
};

ostream& operator<<(ostream &os, const Name_pairs &np) {
    for (const auto &p : np.pairs) os << p.name << ", " << p.age << "\n";
    return os;
}
bool operator==(const Name_pairs &a, const Name_pairs &b) {
    if (a.pairs.size() != b.pairs.size()) return false;
    for (size_t i = 0; i < a.pairs.size(); i++)
        if (a.pairs[i].name != b.pairs[i].name || a.pairs[i].age != b.pairs[i].age)
            return false;
    return true;
}
bool operator!=(const Name_pairs &a, const Name_pairs &b) { return !(a == b); }

void demo() {
    cout << "\n--- [4] Name_pairs rebuilt on top of Name_pair struct ---\n";
    Name_pairs np;
    np.add("Zarina", 22);
    np.add("Ahmed", 25);
    np.add("Bekzat", 19);

    cout << "Before sort:\n" << np;
    np.sort();
    cout << "After sort:\n" << np;
}

} // namespace Ex4

int main() {
    Ex2::demo();
    Ex3::demo();
    Ex4::demo();
    return 0;
}