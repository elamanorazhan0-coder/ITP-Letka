#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <stdexcept>

using namespace std;

// [1] "Try this" exercises are small in-text checks scattered through
// the chapter; the demos below already exercise the same ideas so
// nothing extra is added here.

// [2]/[3]/[4] Jack and Jill (STL version) 
vector<string> read_words(const string& filename) {
    ifstream in(filename);
    if (!in) throw runtime_error("cannot open " + filename);
    vector<string> words;
    string w;
    while (in >> w) words.push_back(w);
    return words;
}

void ex2_3_4_jack_and_jill(const string& filename) {
    vector<string> words = read_words(filename);
    cout << "read " << words.size() << " words: ";
    for (auto& w : words) cout << w << ' ';
    cout << "\n";
}

// [5] << and >> for vector<T> 
template<typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "{ ";
    for (auto& x : v) os << x << ' ';
    return os << "}";
}
template<typename T>
istream& operator>>(istream& is, vector<T>& v) {
    v.clear();
    T x;
    while (is >> x) v.push_back(x);
    is.clear();          // reading until eof leaves the stream in fail state
    return is;
}

// [6]/[8]/[9]/[10] a minimal Document 
struct Document {
    vector<vector<string>> lines;   // lines[i] is the words of line i
};

Document make_document(const string& text) {
    Document doc;
    istringstream iss(text);
    string line;
    while (getline(iss, line)) {
        vector<string> words;
        istringstream ls(line);
        string w;
        while (ls >> w) words.push_back(w);
        doc.lines.push_back(words);
    }
    return doc;
}

// [6] find-and-replace across a whole Document
int find_and_replace(Document& doc, const string& from, const string& to) {
    int count = 0;
    for (auto& line : doc.lines)
        for (auto& w : line)
            if (w == from) { w = to; ++count; }
    return count;
}

// [8] count characters in a Document 
long count_chars(const Document& doc) {
    long total = 0;
    for (auto& line : doc.lines)
        for (auto& w : line) total += w.size() + 1;
    return total;
}

// [9] count words two ways
// (a) whitespace-separated sequence of characters - this is exactly
//     what our Document already stores per word, so counting is trivial
long count_words_whitespace(const Document& doc) {
    long total = 0;
    for (auto& line : doc.lines) total += line.size();
    return total;
}
// (b) sequence of consecutive alphabetic characters -- alpha.numeric
//     is TWO words under this rule , so we must look
//     inside each whitespace-token and split further on non-alpha chars
long count_words_alpha(const Document& doc) {
    long total = 0;
    for (auto& line : doc.lines) {
        for (auto& tok : line) {
            bool in_word = false;
            for (char c : tok) {
                bool is_alpha = isalpha(static_cast<unsigned char>(c));
                if (is_alpha && !in_word) { ++total; in_word = true; }
                else if (!is_alpha) in_word = false;
            }
        }
    }
    return total;
}

// [10] word count where the caller supplies the set of whitespace chars
long count_words_custom_ws(const string& text, const string& ws_chars) {
    long total = 0;
    bool in_word = false;
    for (char c : text) {
        bool is_ws = (ws_chars.find(c) != string::npos);
        if (!is_ws && !in_word) { ++total; in_word = true; }
        else if (is_ws) in_word = false;
    }
    return total;
}

// [7] lexicographically last string 
string lex_last(const vector<string>& v) {
    if (v.empty()) throw runtime_error("lex_last(): empty vector");
    string best = v[0];
    for (auto& s : v) if (s > best) best = s;
    return best;
}

// [11] list<int> -> vector<double>, sorted 
void ex11_list_to_vector(list<int>& lst) {
    vector<double> v(lst.begin(), lst.end());   // range-construct = "copy the elements"

    // verify: same size, same multiset of values
    bool ok = (v.size() == lst.size());
    if (ok) {
        vector<int> from_list(lst.begin(), lst.end());
        vector<double> from_vec = v;
        sort(from_list.begin(), from_list.end());
        sort(from_vec.begin(), from_vec.end());
        for (size_t i = 0; ok && i < from_list.size(); ++i)
            if (from_list[i] != from_vec[i]) ok = false;
    }
    cout << "copy complete and correct: " << boolalpha << ok << "\n";

    sort(v.begin(), v.end());
    cout << "sorted: ";
    for (double x : v) cout << x << ' ';
    cout << "\n";
}

// [12]/[13] a minimal list<T> (as in §19.3) 
// This is a simplified stand-in for the book's own List_iterator-based
// list enough to demonstrate the "one past the end" question
template<typename T>
struct List_elem { T value; List_elem* next; };

// [12] version with a REAL sentinel Link allocated for one-past-the-end.
template<typename T>
class List_real_end {
    List_elem<T>* first_ = nullptr;
    List_elem<T>* last_ = nullptr;   // allocated, but holds no real value
public:
    List_real_end() { last_ = new List_elem<T>{T(), nullptr}; first_ = last_; }
    void push_back(const T& v) {
        List_elem<T>* n = new List_elem<T>{v, last_};
        if (first_ == last_) first_ = n; else {
            List_elem<T>* p = first_;
            while (p->next != last_) p = p->next;
            p->next = n;
        }
    }
    // high() find the largest element -- classic §19.3 example
    T high() const {
        if (first_ == last_) throw runtime_error("high(): empty list");
        T best = first_->value;
        for (List_elem<T>* p = first_; p != last_; p = p->next)
            if (p->value > best) best = p->value;
        return best;
    }
    ~List_real_end() {
        List_elem<T>* p = first_;
        while (p != last_) { List_elem<T>* n = p->next; delete p; p = n; }
        delete last_;
    }
};

// [13] version using nullptr as the  one-past-the-end
// marker instead -- an empty list is then just a single null pointer,
// which is as small as a list can possibly be.
template<typename T>
class List_null_end {
    List_elem<T>* first_ = nullptr;
public:
    void push_back(const T& v) {
        List_elem<T>* n = new List_elem<T>{v, nullptr};
        if (!first_) { first_ = n; return; }
        List_elem<T>* p = first_;
        while (p->next) p = p->next;
        p->next = n;
    }
    T high() const {
        if (!first_) throw runtime_error("high(): empty list");
        T best = first_->value;
        for (List_elem<T>* p = first_; p; p = p->next)
            if (p->value > best) best = p->value;
        return best;
    }
    ~List_null_end() {
        List_elem<T>* p = first_;
        while (p) { List_elem<T>* n = p->next; delete p; p = n; }
    }
};

// [14] Slist (singly-linked, std::list-style) 
// Because there's no `prev` pointer, anything that needs to go
// backward has to go: no rbegin()/rend(), no pop_back() in O(1) (you'd
// have to walk from the front to find the second-to-last element), and
// no O(1) erase given only an iterator to the element itself
template<typename T>
class Slist {
    struct Node { T value; Node* next; };
    Node* head_ = nullptr;
public:
    void push_front(const T& v) { head_ = new Node{v, head_}; }
    template<typename F>
    void for_each(F f) const { for (Node* p = head_; p; p = p->next) f(p->value); }
    ~Slist() { while (head_) { Node* n = head_->next; delete head_; head_ = n; } }
};

// [15] Pvector: vector of pointers, deletes on destruction 
template<typename T>
class Pvector {
    vector<T*> v;
public:
    void push_back(T* p) { v.push_back(p); }
    T* operator[](size_t i) { return v[i]; }
    size_t size() const { return v.size(); }
    ~Pvector() { for (T* p : v) delete p; }
};

// [16] Ovector: like Pvector, but [] / * dereference for you 
template<typename T>
class Ovector {
    vector<T*> v;
public:
    void push_back(T* p) { v.push_back(p); }
    T& operator[](size_t i) { return *v[i]; }   // returns the OBJECT, not the pointer
    size_t size() const { return v.size(); }
    ~Ovector() { for (T* p : v) delete p; }
};

// [17] Ownership_vector: caller picks which elements get deleted 
template<typename T>
class Ownership_vector {
    vector<T*> v;
    vector<bool> owns;
public:
    void push_back(T* p, bool take_ownership) { v.push_back(p); owns.push_back(take_ownership); }
    T& operator[](size_t i) { return *v[i]; }
    size_t size() const { return v.size(); }
    ~Ownership_vector() {
        for (size_t i = 0; i < v.size(); ++i) if (owns[i]) delete v[i];
    }
};

// [18] range-checked iterator for vector 
template<typename T>
class Checked_vector_iter {
    T* p;
    T* begin_;
    T* end_;
public:
    Checked_vector_iter(T* pp, T* b, T* e) : p(pp), begin_(b), end_(e) {}
    T& operator*() const {
        if (p < begin_ || p >= end_) throw out_of_range("iterator out of range");
        return *p;
    }
    Checked_vector_iter& operator++() { ++p; return *this; }
    bool operator!=(const Checked_vector_iter& o) const { return p != o.p; }
};

template<typename T>
Checked_vector_iter<T> checked_begin(vector<T>& v) {
    return Checked_vector_iter<T>(v.data(), v.data(), v.data() + v.size());
}
template<typename T>
Checked_vector_iter<T> checked_end(vector<T>& v) {
    return Checked_vector_iter<T>(v.data() + v.size(), v.data(), v.data() + v.size());
}

// [19] range-checked iterator for list 
// list iterators only need to move one step at a time ,
// so unlike the vector version there's no pointer arithmetic - we just
// track whether we have reached end() and refuse to dereference there
template<typename T>
class Checked_list_iter {
    typename list<T>::iterator it;
    typename list<T>::iterator end_;
public:
    Checked_list_iter(typename list<T>::iterator i, typename list<T>::iterator e) : it(i), end_(e) {}
    T& operator*() const {
        if (it == end_) throw out_of_range("iterator out of range");
        return *it;
    }
    Checked_list_iter& operator++() { ++it; return *this; }
    bool operator!=(const Checked_list_iter& o) const { return it != o.it; }
};

// [20] vector vs list timing experiment 
// Insert N random ints into a container one at a time, always keeping
// it sorted. vector must shift elements on every insert (O(n) per
// insert); list has O(1) splice-in once the spot is found, but finding
// the spot is still a linear scan either way
void ex20_timing_experiment(int N) {
    vector<int> values(N);
    for (int& x : values) x = rand() % N;

    vector<int> vec;
    auto t1 = chrono::steady_clock::now();
    for (int x : values) vec.insert(upper_bound(vec.begin(), vec.end(), x), x);
    auto t2 = chrono::steady_clock::now();

    list<int> lst;
    auto t3 = chrono::steady_clock::now();
    for (int x : values) {
        auto it = lst.begin();
        while (it != lst.end() && *it <= x) ++it;
        lst.insert(it, x);
    }
    auto t4 = chrono::steady_clock::now();

    auto ms = [](auto a, auto b) { return chrono::duration_cast<chrono::milliseconds>(b - a).count(); };
    cout << "N=" << N << "  vector: " << ms(t1, t2) << " ms   list: " << ms(t3, t4) << " ms\n";
}

// main()
int main() {
    cout << "=== [2]-[4] Jack and Jill (STL version) ===\n";
    { ofstream("jack.txt") << "Jack and Jill went up the hill\nto fetch a pail of water"; }
    ex2_3_4_jack_and_jill("jack.txt");

    cout << "\n=== [5] << / >> for vector ===\n";
    vector<int> vv{1,2,3};
    cout << vv << "\n";
    istringstream iss("4 5 6");
    vector<int> vv2;
    iss >> vv2;
    cout << vv2 << "\n";

    cout << "\n=== [6],[8],[9],[10] Document ===\n";
    Document doc = make_document("The quick brown fox\nalpha.numeric as12b jumps");
    cout << "replacements: " << find_and_replace(doc, "fox", "dog") << "\n";
    cout << "chars: " << count_chars(doc) << "\n";
    cout << "words (whitespace def): " << count_words_whitespace(doc) << "\n";
    cout << "words (alpha-run def): " << count_words_alpha(doc) << "\n";
    cout << "words with custom ws \",\": " << count_words_custom_ws("a,b,c d", ", ") << "\n";

    cout << "\n=== [7] lexicographically last ===\n";
    cout << lex_last({"pear", "apple", "zebra", "mango"}) << "\n";

    cout << "\n=== [11] list<int> -> vector<double> ===\n";
    list<int> li{5, 2, 8, 1};
    ex11_list_to_vector(li);

    cout << "\n=== [12] list with real one-past-end Link, high() ===\n";
    List_real_end<int> lre;
    lre.push_back(3); lre.push_back(9); lre.push_back(4);
    cout << "high() = " << lre.high() << "\n";

    cout << "\n=== [13] list with nullptr as one-past-end ===\n";
    List_null_end<int> lne;
    lne.push_back(3); lne.push_back(9); lne.push_back(4);
    cout << "high() = " << lne.high() << "\n";

    cout << "\n=== [14] Slist ===\n";
    Slist<string> sl;
    sl.push_front("c"); sl.push_front("b"); sl.push_front("a");
    sl.for_each([](const string& s) { cout << s << ' '; });
    cout << "\n";

    cout << "\n=== [15] Pvector ===\n";
    { Pvector<string> pv; pv.push_back(new string("x")); pv.push_back(new string("y"));
      for (size_t i = 0; i < pv.size(); ++i) cout << *pv[i] << ' ';
      cout << "\n"; }   // strings deleted automatically here

    cout << "\n=== [16] Ovector ===\n";
    { Ovector<string> ov; ov.push_back(new string("p")); ov.push_back(new string("q"));
      for (size_t i = 0; i < ov.size(); ++i) cout << ov[i] << ' ';   // no * needed
      cout << "\n"; }

    cout << "\n=== [17] Ownership_vector ===\n";
    { string not_owned = "stack string";
      Ownership_vector<string> ownv;
      ownv.push_back(new string("heap string"), true);   // will be deleted
      ownv.push_back(&not_owned, false);                 // will NOT be deleted
      for (size_t i = 0; i < ownv.size(); ++i) cout << ownv[i] << ' ';
      cout << "\n"; }

    cout << "\n=== [18] range-checked vector iterator ===\n";
    vector<int> rv{1,2,3};
    for (auto it = checked_begin(rv); it != checked_end(rv); ++it) cout << *it << ' ';
    cout << "\n";
    try { auto it = checked_end(rv); *it; } catch (const exception& e) { cout << "caught: " << e.what() << "\n"; }

    cout << "\n=== [19] range-checked list iterator ===\n";
    list<int> rl{7,8,9};
    Checked_list_iter<int> cit(rl.begin(), rl.end());
    Checked_list_iter<int> cend(rl.end(), rl.end());
    for (; cit != cend; ++cit) cout << *cit << ' ';
    cout << "\n";

    cout << "\n=== [20] vector vs list timing ===\n";
    ex20_timing_experiment(3000);   // kept small so the demo finishes quickly

    return 0;
}