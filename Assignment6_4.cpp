#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;

// [1] "Try this" exercises are small in-text checks; already covered by
// the demos throughout the chapter, nothing separate to add here.


// [3] count() reimplemented 
template<typename Iter, typename T>
int my_count(Iter first, Iter last, const T& value) {
    int n = 0;
    for (; first != last; ++first)
        if (*first == value) ++n;
    return n;
}

// [4] count_if() reimplemented 
template<typename Iter, typename Pred>
int my_count_if(Iter first, Iter last, Pred pred) {
    int n = 0;
    for (; first != last; ++first)
        if (pred(*first)) ++n;
    return n;
}

// [5] find()/count() without end() as "not found" 
// Without end() to signal failure, we need a different way to say
// "nothing matched". We use bool return + an out-parameter for the
// found iterator -- the caller can no longer accidentally use a
// "not found" iterator by mistake, since there's nothing to dereference.
template<typename Iter, typename T>
bool my_find2(Iter first, Iter last, const T& value, Iter& result) {
    for (; first != last; ++first)
        if (*first == value) { result = first; return true; }
    return false;
}

template<typename Iter, typename T>
int my_count2(Iter first, Iter last, const T& value) {
    int n = 0;
    for (; first != last; ++first) if (*first == value) ++n;
    return n;
}

// [6] binary search, no std:: version 
// vector<int>: random access lets us jump straight to the midpoint.
bool binary_search_vec(const vector<int>& v, int target) {
    int lo = 0, hi = static_cast<int>(v.size()) - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (v[mid] == target) return true;
        if (v[mid] < target) lo = mid + 1; else hi = mid - 1;
    }
    return false;
}

// list<string>: no random access, so we can't jump to a midpoint
// cheaply -- getting there is itself an O(n) walk. The two functions
// end up looking almost nothing alike, because "find the midpoint" is
// where all the actual work is, and that step has completely different
// costs for the two containers. Without knowing the STL's iterator
// categories, you'd probably not expect these to differ this much --
// they'd "resemble" each other only in the abstract idea, not the code.
bool binary_search_list(const list<string>& lst, const string& target) {
    auto lo = lst.begin();
    int n = static_cast<int>(lst.size());
    while (n > 0) {
        auto mid = lo;
        advance(mid, n / 2);
        if (*mid == target) return true;
        if (*mid < target) { lo = mid; ++lo; n = n - n / 2 - 1; }
        else n = n / 2;
    }
    return false;
}

// [7]/[8] Order / Purchase
struct Purchase {
    string name;       // product name
    double unit_price;
    int count;
    double value() const { return unit_price * count; }   // [8]
};

struct Order {
    string customer_name, address, date;
    vector<Purchase> purchases;
};

ostream& operator<<(ostream& os, const Purchase& p) {
    return os << p.name << ";" << p.unit_price << ";" << p.count;
}
istream& operator>>(istream& is, Purchase& p) {
    string line;
    if (!getline(is, line)) return is;
    istringstream ls(line);
    string tok;
    getline(ls, p.name, ';');
    getline(ls, tok, ';'); p.unit_price = stod(tok);
    getline(ls, tok, ';'); p.count = stoi(tok);
    return is;
}

ostream& operator<<(ostream& os, const Order& o) {
    os << o.customer_name << "|" << o.address << "|" << o.date << "|" << o.purchases.size() << "\n";
    for (auto& p : o.purchases) os << p << "\n";
    return os;
}
istream& operator>>(istream& is, Order& o) {
    string header;
    if (!getline(is, header)) return is;
    istringstream hs(header);
    string tok;
    getline(hs, o.customer_name, '|');
    getline(hs, o.address, '|');
    getline(hs, o.date, '|');
    getline(hs, tok, '|');
    int n = stoi(tok);
    o.purchases.resize(n);
    for (int i = 0; i < n; ++i) is >> o.purchases[i];
    return is;
}

// writes/reads a whole sequence of Orders to/from a file
template<typename Container>
void write_orders(const string& filename, const Container& orders) {
    ofstream out(filename);
    for (auto& o : orders) out << o;
}
template<typename Container>
void read_orders(const string& filename, Container& orders) {
    ifstream in(filename);
    Order o;
    while (in >> o) orders.push_back(o);
}

double order_total(const Order& o) {
    double sum = 0;
    for (auto& p : o.purchases) sum += p.value();
    return sum;
}

void ex7_8_orders_demo() {
    vector<Order> file1;
    for (int i = 0; i < 10; ++i) {
        Order o;
        o.customer_name = "Customer" + to_string(9 - i);   // reverse-ish so sort has work to do
        o.address = "Addr" + to_string(i % 4);
        o.date = "2024-01-0" + to_string(1 + i % 9);
        o.purchases.push_back({"Widget", 2.5, 3 + i});
        file1.push_back(o);
    }
    write_orders("orders1.txt", file1);

    vector<Order> read1;
    read_orders("orders1.txt", read1);
    sort(read1.begin(), read1.end(), [](auto& a, auto& b) { return a.customer_name < b.customer_name; });
    write_orders("orders1_sorted.txt", read1);
    cout << "file1: read " << read1.size() << " orders, sorted by customer name\n";

    list<Order> file2;
    for (int i = 0; i < 10; ++i) {
        Order o;
        if (i < 3) o = file1[i];                      // about a third overlap with file1
        else { o.customer_name = "Other" + to_string(i); o.address = "Addr" + to_string(i % 4);
               o.date = "2024-02-01"; o.purchases.push_back({"Gadget", 9.99, i + 1}); }
        file2.push_back(o);
    }
    file2.sort([](const Order& a, const Order& b) { return a.address < b.address; });
    write_orders("orders2_sorted.txt", file2);
    cout << "file2: " << file2.size() << " orders, sorted by address\n";

    // std::merge needs both inputs already sorted the SAME way; re-sort
    // file2 by name (matching file1's order) purely for the merge step.
    list<Order> file2_by_name(file2.begin(), file2.end());
    file2_by_name.sort([](const Order& a, const Order& b) { return a.customer_name < b.customer_name; });
    vector<Order> merged;
    merge(read1.begin(), read1.end(), file2_by_name.begin(), file2_by_name.end(),
          back_inserter(merged), [](const Order& a, const Order& b) { return a.customer_name < b.customer_name; });
    write_orders("orders_merged.txt", merged);
    cout << "merged: " << merged.size() << " orders total\n";

    double total1 = 0; for (auto& o : read1) total1 += order_total(o);
    double total2 = 0; for (auto& o : file2) total2 += order_total(o);
    cout << "[8] total value: file1=" << total1 << " file2=" << total2 << "\n";
}

// [9]/[10]/[13] A GUI for entering/querying Orders 
// would sit on top of the console logic above using the book's FLTK-
// based GUI chapters, which this console build doesn't link against.

// [11] clean up a text file 
string clean_text_full(const string& text) {
    static const map<string,string> contractions = {
        {"don't","do not"}, {"can't","cannot"}, {"won't","will not"}, {"it's","it is"}
    };
    // Pass 1: expand contractions word by word (need the apostrophe intact for lookup)
    istringstream iss(text);
    string word, expanded;
    while (iss >> word) {
        string lower = word;
        for (char& c : lower) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        auto it = contractions.find(lower);
        expanded += (it != contractions.end()) ? it->second : lower;
        expanded += ' ';
    }
    // Pass 2 punctuation -> whitespace, lowercase already done above
    string cleaned;
    for (char c : expanded)
        cleaned += isalpha(static_cast<unsigned char>(c)) || isspace(static_cast<unsigned char>(c)) ? c : ' ';

    // Pass 3 crude plural removal -- only strip a trailing 's' (not "es",
    // not "ies"); good enough per the exercise's own "don't be too
    // ambitious" instruction.
    istringstream iss2(cleaned);
    string out;
    while (iss2 >> word) {
        if (word.size() > 1 && word.back() == 's') word.pop_back();
        out += word + ' ';
    }
    return out;
}

// [12] word-query program using the cleaned text 
void ex12_word_queries(const string& cleaned_text) {
    istringstream iss(cleaned_text);
    vector<string> words;
    string w;
    while (iss >> w) words.push_back(w);

    map<string,int> counts;
    for (auto& word : words) ++counts[word];

    cout << "occurrences of \"ship\": " << counts["ship"] << "\n";

    auto most_frequent = max_element(counts.begin(), counts.end(),
        [](auto& a, auto& b) { return a.second < b.second; });
    if (most_frequent != counts.end())
        cout << "most frequent: " << most_frequent->first << " (" << most_frequent->second << ")\n";

    auto longest = max_element(words.begin(), words.end(),
        [](auto& a, auto& b) { return a.size() < b.size(); });
    auto shortest = min_element(words.begin(), words.end(),
        [](auto& a, auto& b) { return a.size() < b.size(); });
    if (longest != words.end()) cout << "longest: " << *longest << "\n";
    if (shortest != words.end()) cout << "shortest: " << *shortest << "\n";

    cout << "words starting with 's': ";
    for (auto& [word, n] : counts) if (!word.empty() && word[0] == 's') cout << word << ' ';
    cout << "\n";

    cout << "four-letter words: ";
    for (auto& [word, n] : counts) if (word.size() == 4) cout << word << ' ';
    cout << "\n";
}

// main()
int main() {
    cout << "=== [3] my_count ===\n";
    vector<int> v{1,2,3,2,2,5};
    cout << my_count(v.begin(), v.end(), 2) << "\n";

    cout << "\n=== [4] my_count_if ===\n";
    cout << my_count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }) << "\n";

    cout << "\n=== [5] find/count without end() ===\n";
    vector<int>::iterator res;
    if (my_find2(v.begin(), v.end(), 3, res)) cout << "found 3 at offset " << (res - v.begin()) << "\n";
    else cout << "not found\n";
    cout << "count of 9: " << my_count2(v.begin(), v.end(), 9) << " (0 means absent, no end() needed)\n";

    cout << "\n=== [6] binary search: vector<int> and list<string> ===\n";
    vector<int> sorted_ints{1,3,5,7,9,11,13};
    cout << "search 7: " << boolalpha << binary_search_vec(sorted_ints, 7) << "\n";
    cout << "search 8: " << binary_search_vec(sorted_ints, 8) << "\n";
    list<string> sorted_strs{"ant","bee","cat","dog","eel"};
    cout << "search \"cat\": " << binary_search_list(sorted_strs, "cat") << "\n";
    cout << "search \"fox\": " << binary_search_list(sorted_strs, "fox") << "\n";

    cout << "\n=== [7]/[8] Order/Purchase: read, sort, merge, total value ===\n";
    ex7_8_orders_demo();

    cout << "\n=== [11] clean up a text file ===\n";
    string raw = "The ships don't sail; the SHIP can't move! Ships and dogs.";
    string cleaned = clean_text_full(raw);
    cout << cleaned << "\n";

    cout << "\n=== [12] word queries on the cleaned text ===\n";
    ex12_word_queries(cleaned);

    return 0;
}