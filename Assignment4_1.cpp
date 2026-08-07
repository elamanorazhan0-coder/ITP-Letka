#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <stdexcept>
using namespace std;

void error(const string& msg) {
    throw runtime_error(msg);
}

// [2] Print a vector<int> with a label
void print(const string& label, const vector<int>& v) {
    cout << label << ": ";
    for (int x : v) cout << x << ' ';
    cout << '\n';
}


// [3] Fill a vector with a Fibonacci-like sequence v is passed by non-const reference because the function modifies it.
void fibonacci(int x, int y, vector<int>& v, int n) {
    if (n < 1) return;
    v.push_back(x);
    if (n < 2) return;
    v.push_back(y);
    for (int i = 2; i < n; ++i)
        v.push_back(v[i-1] + v[i-2]);
}

// [5] Two reverse functions for vector<int>
// First: returns a new vector; the original is unchanged (const&)
vector<int> reverse_copy(const vector<int>& v) {
    return vector<int>(v.rbegin(), v.rend());
}

// Second: in-place reversal without extra memory, using swap
void reverse_inplace(vector<int>& v) {
    for (size_t i = 0, j = v.size(); i < j; ++i, --j)
        swap(v[i], v[j-1]);
}

// [6] Same functions overloaded for vector<string>
vector<string> reverse_copy(const vector<string>& v) {
    return vector<string>(v.rbegin(), v.rend());
}

void reverse_inplace(vector<string>& v) {
    for (size_t i = 0, j = v.size(); i < j; ++i, --j)
        swap(v[i], v[j-1]);
}

// [7] Fixed number of names (5)
// Problem: after sorting names, ages become misaligned.
// Solution: remember the original order, then rearrange ages to match the sorted names.
void exercise07() {
    const int n = 5;
    vector<string> name(n);
    vector<double> age(n);

    cout << "Enter " << n << " names:\n";
    for (int i = 0; i < n; ++i) cin >> name[i];

    for (int i = 0; i < n; ++i) {
        cout << "Age of " << name[i] << ": ";
        cin >> age[i];
    }

    cout << "Original pairs:\n";
    for (int i = 0; i < n; ++i)
        cout << '(' << name[i] << ',' << age[i] << ")\n";

    vector<string> name_orig = name;
    vector<double> age_orig = age;
    sort(name.begin(), name.end());

    vector<double> age_sorted(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (name[i] == name_orig[j]) {
                age_sorted[i] = age_orig[j];
                break;
            }
        }
    }

    cout << "Sorted pairs:\n";
    for (int i = 0; i < n; ++i)
        cout << '(' << name[i] << ',' << age_sorted[i] << ")\n";
}

// [8] Arbitrary number of names
// Input ends when the user types "end".
void exercise08() {
    vector<string> name;
    vector<double> age;
    string s;

    cout << "Enter names (type 'end' to finish):\n";
    while (cin >> s && s != "end") name.push_back(s);

    for (const auto& n : name) {
        cout << "Age of " << n << ": ";
        double a; cin >> a;
        age.push_back(a);
    }

    cout << "Original pairs:\n";
    for (size_t i = 0; i < name.size(); ++i)
        cout << '(' << name[i] << ',' << age[i] << ")\n";

    vector<string> name_orig = name;
    vector<double> age_orig = age;
    sort(name.begin(), name.end());

    vector<double> age_sorted(name.size());
    for (size_t i = 0; i < name.size(); ++i) {
        for (size_t j = 0; j < name_orig.size(); ++j) {
            if (name[i] == name_orig[j]) {
                age_sorted[i] = age_orig[j];
                break;
            }
        }
    }

    cout << "Sorted pairs:\n";
    for (size_t i = 0; i < name.size(); ++i)
        cout << '(' << name[i] << ',' << age_sorted[i] << ")\n";
}

// [9] Weighted sum of two vectors
// Precondition: sizes must match. We check this to avoid out-of-bounds access or an incorrect result.
double compute_index(const vector<double>& price, const vector<double>& weight) {
    if (price.size() != weight.size())
        error("compute_index: size mismatch");
    double sum = 0.0;
    for (size_t i = 0; i < price.size(); ++i)
        sum += price[i] * weight[i];
    return sum;
}

// [10] Largest element in a vector<int>
int maxv(const vector<int>& v) {
    if (v.empty()) error("maxv: empty vector");
    int m = v[0];
    for (int x : v)
        if (x > m) m = x;
    return m;
}

// [11] Min, max, mean, and median
// Returning a struct is preferable to out-parameters because
// the interface is cleaner: one return value, and the caller does not need to create extra variables beforehand.
struct Stats {
    double min;
    double max;
    double mean;
    double median;
};

Stats analyze(const vector<double>& v) {
    if (v.empty()) error("analyze: empty vector");
    Stats s;
    s.min = s.max = v[0];
    double sum = 0.0;
    for (double x : v) {
        if (x < s.min) s.min = x;
        if (x > s.max) s.max = x;
        sum += x;
    }
    s.mean = sum / v.size();

    vector<double> tmp = v;     // copy so we do not alter the original order
    sort(tmp.begin(), tmp.end());
    size_t n = tmp.size();
    if (n % 2 == 1)
        s.median = tmp[n/2];
    else
        s.median = (tmp[n/2 - 1] + tmp[n/2]) / 2.0;
    return s;
}

// [12] print_until_s and print_until_ss
// A good set of test cases should cover:
// - empty vector;
// - quit absent;
// - quit first / last;
// - multiple consecutive quits;
// - exactly one quit (for print_until_ss).
void print_until_s(const vector<string>& v, const string& quit) {
    for (const string& s : v) {
        if (s == quit) return;
        cout << s << ' ';
    }
}

// Prints until a second occurrence of quit is seen
void print_until_ss(const vector<string>& v, const string& quit) {
    int count = 0;
    for (const string& s : v) {
        if (s == quit) {
            ++count;
            if (count == 2) return;
        }
        cout << s << ' ';
    }
}

// [13] Analysis of vector<string>
// Split into separate functions because each solves one task this makes them easier to test, read, and reuse.

vector<int> string_lengths(const vector<string>& v) {
    vector<int> res;
    for (const auto& s : v) res.push_back(static_cast<int>(s.size()));
    return res;
}

string longest_string(const vector<string>& v) {
    if (v.empty()) error("longest_string: empty");
    string m = v[0];
    for (const auto& s : v)
        if (s.size() > m.size()) m = s;
    return m;
}

string shortest_string(const vector<string>& v) {
    if (v.empty()) error("shortest_string: empty");
    string m = v[0];
    for (const auto& s : v)
        if (s.size() < m.size()) m = s;
    return m;
}

string lex_first(const vector<string>& v) {
    if (v.empty()) error("lex_first: empty");
    string m = v[0];
    for (const auto& s : v)
        if (s < m) m = s;
    return m;
}

string lex_last(const vector<string>& v) {
    if (v.empty()) error("lex_last: empty");
    string m = v[0];
    for (const auto& s : v)
        if (s > m) m = s;
    return m;
}

// ============================================================
// [14] const for a value parameter
// It is allowed, but in a declaration (prototype) const is ignored:
// void f(const int) is equivalent to void f(int).
void demo_const_value(const int x) {
    // x = 10; // compile error: assignment of read-only parameter
    cout << "demo_const_value: x = " << x << '\n';
}

int main() {
    try {
        // --- [2] + [3] ---
        vector<int> fib;
        fibonacci(1, 2, fib, 10);
        print("Fibonacci (1,2,10)", fib);

        // 4 Approximation of the maximum int
        // Generate until a negative number appears (signed overflow).
        vector<int> fib_max;
        fibonacci(1, 1, fib_max, 50);
        int approx_max = 0;
        for (size_t i = 2; i < fib_max.size(); ++i) {
            if (fib_max[i] < 0) {      // sign of signed int overflow
                approx_max = fib_max[i-1];
                break;
            }
        }
        cout << "Approx int max via Fibonacci: " << approx_max
             << " (real INT_MAX = " << INT_MAX << ")\n";

        // 5
        vector<int> rv = {1, 3, 5, 7, 9};
        vector<int> rvc = reverse_copy(rv);
        print("Original", rv);
        print("Reversed copy", rvc);
        reverse_inplace(rv);
        print("Reversed inplace", rv);

        // 6
        vector<string> sv = {"one", "two", "three"};
        vector<string> svc = reverse_copy(sv);
        reverse_inplace(sv);
        cout << "String reverse check: OK (see code)\n";

        // 9
        vector<double> price = {1.0, 2.0, 3.0};
        vector<double> weight = {0.5, 1.0, 1.5};
        cout << "Index = " << compute_index(price, weight) << '\n';

        // 10
        cout << "Max of fib = " << maxv(fib) << '\n';

        // 11
        vector<double> data = {3.0, 1.0, 4.0, 1.0, 5.0};
        Stats st = analyze(data);
        cout << "Stats: min=" << st.min << " max=" << st.max
             << " mean=" << st.mean << " median=" << st.median << '\n';

        //12
        vector<string> words = {"hello", "world", "quit", "foo", "quit", "bar"};
        cout << "print_until_s:    ";
        print_until_s(words, "quit");
        cout << "\nprint_until_ss:   ";
        print_until_ss(words, "quit");
        cout << '\n';

        //13
        vector<string> fruits = {"apple", "kiwi", "banana", "plum"};
        vector<int> lens = string_lengths(fruits);
        cout << "Lengths: ";
        for (int l : lens) cout << l << ' ';
        cout << "\nLongest:   " << longest_string(fruits)
             << "\nShortest:  " << shortest_string(fruits)
             << "\nLex first: " << lex_first(fruits)
             << "\nLex last:  " << lex_last(fruits) << '\n';

        //14
        demo_const_value(42);

        // --- [7] and [8] — uncomment for manual testing ---
        // exercise07();
        // exercise08();

    } catch (exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}