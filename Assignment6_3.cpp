#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <unordered_set>
#include <set>
#include <vector>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <random>

using namespace std;

// [1] case/punctuation-insensitive word count 
map<string, int> word_count_clean(const string& text) {
    map<string, int> counts;
    string word;
    for (char c : text) {
        if (isalpha(static_cast<unsigned char>(c))) {
            word += static_cast<char>(tolower(static_cast<unsigned char>(c)));
        } else if (!word.empty()) {
            ++counts[word];
            word.clear();
        }
        // any punctuation/digit/space just acts as a word break, per the exercise
    }
    if (!word.empty()) ++counts[word];
    return counts;
}

// [2] Fruit example: set<Fruit> vs set<Fruit*, cmp> 
struct Fruit {
    string name;
    double price;
};

// value-based set: elements are copied in, comparison is by name
struct Fruit_less {
    bool operator()(const Fruit& a, const Fruit& b) const { return a.name < b.name; }
};

// pointer-based set: no copies, but we must teach the set how to
// compare through the pointer instead of comparing addresses
struct Fruit_comparison {
    bool operator()(const Fruit* a, const Fruit* b) const { return a->name < b->name; }
};

void ex2_fruit_demo() {
    // value version: the set OWNS its own copies of the Fruits
    set<Fruit, Fruit_less> by_value;
    by_value.insert(Fruit{"apple", 0.5});
    by_value.insert(Fruit{"banana", 0.3});

    // pointer version: the set just stores addresses; whoever created
    // the Fruits is still responsible for their lifetime
    Fruit apple{"apple", 0.5}, banana{"banana", 0.3};
    set<Fruit*, Fruit_comparison> by_pointer;
    by_pointer.insert(&apple);
    by_pointer.insert(&banana);

    cout << "by value: ";
    for (auto& f : by_value) cout << f.name << ' ';
    cout << "\nby pointer: ";
    for (auto* f : by_pointer) cout << f->name << ' ';
    cout << "\n";
    cout << "difference: the value-based set duplicates every Fruit into\n"
         << "its own storage (safe, but costs a copy per insert); the\n"
         << "pointer-based set is cheaper to insert into and lets us mutate\n"
         << "the original Fruit through the pointer, but it's now on us to\n"
         << "make sure the pointed-to Fruits outlive the set.\n";
}

// [3] word frequency sorted by frequency 
void ex3_frequency_sorted(const string& text) {
    map<string, int> counts = word_count_clean(text);
    vector<pair<string,int>> items(counts.begin(), counts.end());
    sort(items.begin(), items.end(),
         [](auto& a, auto& b) { return a.second > b.second; });   // most frequent first
    for (auto& [word, n] : items) cout << n << ": " << word << "\n";
}

// [4]-[7] timing: vector / set / unordered_set of ints and strings 
vector<int> random_ints(int n) {
    vector<int> v(n);
    mt19937 gen(42);            // fixed seed so results are reproducible for the defense
    uniform_int_distribution<int> dist(0, n);
    for (int& x : v) x = dist(gen);
    return v;
}

vector<string> random_strings(int n) {
    mt19937 gen(42);
    uniform_int_distribution<int> len_dist(4, 24);
    uniform_int_distribution<int> ch_dist(0, 35);   // 26 letters + 10 digits
    vector<string> v(n);
    for (auto& s : v) {
        int len = len_dist(gen);
        s.resize(len);
        for (char& c : s) {
            int k = ch_dist(gen);
            c = (k < 26) ? char('a' + k) : char('0' + (k - 26));
        }
    }
    return v;
}

template<typename Container, typename Src>
long time_build_ms(Src&& src) {
    auto t1 = chrono::steady_clock::now();
    Container c(src.begin(), src.end());
    auto t2 = chrono::steady_clock::now();
    (void)c;
    return chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();
}

void ex4_7_timing_demo(int N) {
    // the exercise asks for a million+ integers; N is kept smaller
    // here so the demo finishes quickly during a defense, but the code
    // scales to any N -- just change the argument.
    vector<int> ints = random_ints(N);

    auto t1 = chrono::steady_clock::now();
    vector<int> vcopy = ints;
    auto t2 = chrono::steady_clock::now();
    sort(vcopy.begin(), vcopy.end());
    auto t3 = chrono::steady_clock::now();
    cout << "[4] read " << chrono::duration_cast<chrono::milliseconds>(t2-t1).count()
         << " ms, sort " << chrono::duration_cast<chrono::milliseconds>(t3-t2).count() << " ms\n";

    cout << "[5] set<int> build: " << time_build_ms<set<int>>(ints) << " ms\n";
    cout << "[6] unordered_set<int> build: " << time_build_ms<unordered_set<int>>(ints) << " ms\n";
    cout << "    (set has to keep everything ordered as it inserts, so it's\n"
         << "     typically slower than the hash-based unordered_set here.)\n";

    vector<string> strs = random_strings(N);
    cout << "[7] same three timings with strings:\n";
    auto s1 = chrono::steady_clock::now();
    vector<string> scopy = strs;
    auto s2 = chrono::steady_clock::now();
    sort(scopy.begin(), scopy.end());
    auto s3 = chrono::steady_clock::now();
    cout << "    read " << chrono::duration_cast<chrono::milliseconds>(s2-s1).count()
         << " ms, sort " << chrono::duration_cast<chrono::milliseconds>(s3-s2).count() << " ms\n";
    cout << "    set<string> build: " << time_build_ms<set<string>>(strs) << " ms\n";
    cout << "    unordered_set<string> build: " << time_build_ms<unordered_set<string>>(strs) << " ms\n";
}

// [8]/[9] (name,height) pairs 
map<string,int> ex8_read_name_height(const string& infile) {
    ifstream in(infile);
    map<string,int> m;     // map keeps names lexicographically sorted for free
    string name; int height;
    while (in >> name >> height) m[name] = height;
    return m;
}

void ex8_print_by_name(const map<string,int>& m) {
    for (auto& [name, h] : m) cout << name << " : " << h << "\n";
}

void ex9_print_by_height_desc(const map<string,int>& m) {
    vector<pair<string,int>> v(m.begin(), m.end());
    sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.second > b.second; });
    for (auto& [name, h] : v) cout << name << " : " << h << "\n";
}

// [10] unique lines from a file 
void ex10_unique_lines(const string& infile) {
    ifstream in(infile);
    map<string,int> seen;
    string line;
    while (getline(in, line))
        if (!seen[line]++)          // prints exactly the AWK !a[$0]++ idiom
            cout << line << "\n";
}

// [11] English<->Spanish word map 
void ex11_translation_demo(const vector<pair<string,string>>& pairs) {
    map<string,string> eng_to_spa, spa_to_eng;
    for (auto& [e, s] : pairs) { eng_to_spa[e] = s; spa_to_eng[s] = e; }

    cout << "English order:\n";
    for (auto& [e, s] : eng_to_spa) cout << "  " << e << " -> " << s << "\n";
    cout << "Spanish order:\n";
    for (auto& [s, e] : spa_to_eng) cout << "  " << s << " -> " << e << "\n";
}

// [12] same, but words can have multiple meanings 
void ex12_translation_multimap_demo() {
    multimap<string,string> eng_to_spa;
    eng_to_spa.insert({"sheet", "hoja"});
    eng_to_spa.insert({"leaf", "hoja"});
    eng_to_spa.insert({"bank", "banco"});
    eng_to_spa.insert({"bench", "banco"});

    for (auto& [e, s] : eng_to_spa) cout << e << " -> " << s << "\n";

    cout << "all Spanish words meaning 'sheet' or synonyms sharing its translation:\n";
    auto range = eng_to_spa.equal_range("sheet");
    for (auto it = range.first; it != range.second; ++it) cout << "  " << it->second << "\n";
}

// main()
int main() {
    string sample = "The Quick, brown fox! Jumps over the LAZY dog. THE dog barks.";

    cout << "=== [1] case/punctuation-insensitive word count ===\n";
    for (auto& [w, n] : word_count_clean(sample)) cout << w << ": " << n << "\n";

    cout << "\n=== [2] Fruit: set<Fruit> vs set<Fruit*, cmp> ===\n";
    ex2_fruit_demo();

    cout << "\n=== [3] word frequency, sorted by count ===\n";
    ex3_frequency_sorted(sample);

    cout << "\n=== [4]-[7] timing experiments (small N for a fast demo) ===\n";
    ex4_7_timing_demo(20000);

    cout << "\n=== [8]/[9] (name,height) pairs ===\n";
    { ofstream f("heights.txt"); f << "Bob 180\nAlice 165\nCharlie 190\n"; }
    map<string,int> heights = ex8_read_name_height("heights.txt");
    cout << "by name:\n"; ex8_print_by_name(heights);
    cout << "by height (desc):\n"; ex9_print_by_height_desc(heights);

    cout << "\n=== [10] unique lines ===\n";
    { ofstream f("lines.txt") ; f << "apple\nbanana\napple\ncherry\nbanana\napple\n"; }
    ex10_unique_lines("lines.txt");

    cout << "\n=== [11] English<->Spanish map ===\n";
    ex11_translation_demo({{"cat","gato"}, {"dog","perro"}, {"house","casa"}});

    cout << "\n=== [12] multimap for words with several meanings ===\n";
    ex12_translation_multimap_demo();

    return 0;
}