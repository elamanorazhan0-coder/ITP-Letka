#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>
#include <set>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <numeric>

using namespace std;

// [1] 
void ex1_to_lowercase(const string& infile, const string& outfile) {
    ifstream in(infile);
    if (!in) { cerr << "ex1: cannot open " << infile << "\n"; return; }
    ofstream out(outfile);
    char c;
    // Read char-by-char (not line-by-line) so newlines and spacing
    // in the original file are preserved exactly
    while (in.get(c))
        out.put(static_cast<char>(tolower(static_cast<unsigned char>(c))));
}

// [2] 
void ex2_find_word(const string& infile, const string& word) {
    ifstream in(infile);
    if (!in) { cerr << "ex2: cannot open " << infile << "\n"; return; }
    string line;
    int lineno = 0;
    while (getline(in, line)) {
        ++lineno;
        if (line.find(word) != string::npos)
            cout << lineno << ": " << line << "\n";
    }
}

// [3] 
void ex3_disemvowel(const string& infile, const string& outfile) {
    ifstream in(infile);
    if (!in) { cerr << "ex3: cannot open " << infile << "\n"; return; }
    ofstream out(outfile);
    string vowels = "aeiouAEIOU";
    char c;
    while (in.get(c))
        if (vowels.find(c) == string::npos)  // keep everything that is not a vowel
            out.put(c);
}

// [4] 
void ex4_multi_input() {
    cout << "ex4: enter integers using 0x (hex), 0 (octal) or plain (decimal)\n"
         << "     prefixes, separated by whitespace. End with a non-number.\n";
    int n;
    while (cin >> n) {
        cout << setw(10) << n << " decimal\n";
    }
    cin.clear();
}

// Non-interactive version used for automated demonstration/testing.
void ex4_multi_input_demo() {
    istringstream iss("0x43 0123 65");
    iss.unsetf(ios::basefield);   // see comment in ex4_multi_input()
    int n;
    while (iss >> n)
        cout << setw(10) << n << " decimal\n";
}

// [5] 
void classify_char(char c) {
    cout << "  '" << c << "': ";
    if (isalpha(static_cast<unsigned char>(c))) cout << "alpha ";
    if (isdigit(static_cast<unsigned char>(c))) cout << "digit ";
    if (isalnum(static_cast<unsigned char>(c))) cout << "alnum ";
    if (isspace(static_cast<unsigned char>(c))) cout << "space ";
    if (ispunct(static_cast<unsigned char>(c))) cout << "punct ";
    if (isupper(static_cast<unsigned char>(c))) cout << "upper ";
    if (islower(static_cast<unsigned char>(c))) cout << "lower ";
    if (iscntrl(static_cast<unsigned char>(c))) cout << "cntrl ";
    cout << "\n";
}

void ex5_classify_strings() {
    cout << "ex5: type words (Ctrl+D / Ctrl+Z to stop) or run the demo:\n";
    string s;
    while (cin >> s)
        for (char c : s) classify_char(c);
}

void ex5_classify_demo() {
    for (string s : {"x1", "Hi!"})
        for (char c : s) classify_char(c);
}

// [6] 
string ex6_replace_punct(const string& s) {
    string result = s;
    bool inside_quotes = false;
    string punct = ".,;?-'";
    for (char& c : result) {
        if (c == '"') { inside_quotes = !inside_quotes; continue; }
        if (!inside_quotes && punct.find(c) != string::npos)
            c = ' ';
    }
    return result;
}

// [7] 
string ex7_expand_and_clean(const string& s) {
    // Small dictionary is enough to demonstrate the technique; a real
    // program would load this from a file.
    static const map<string, string> contractions = {
        {"don't", "do not"}, {"can't", "cannot"}, {"won't", "will not"},
        {"it's", "it is"}, {"isn't", "is not"}, {"i'm", "i am"},
        {"didn't", "did not"}, {"doesn't", "does not"}
    };

    istringstream iss(s);
    string word, out;
    while (iss >> word) {
        string lower = word;
        for (char& c : lower) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        // strip trailing punctuation (but not hyphen/apostrophe) before lookup
        string key = lower;
        while (!key.empty() && ispunct(static_cast<unsigned char>(key.back())) && key.back() != '-' && key.back() != '\'')
            key.pop_back();

        auto it = contractions.find(key);
        if (it != contractions.end()) {
            out += it->second;
        } else {
            for (char c : lower)
                out += (ispunct(static_cast<unsigned char>(c)) && c != '-') ? ' ' : c;
        }
        out += ' ';
    }
    if (!out.empty()) out.pop_back();
    return out;
}

// [8] 
vector<string> ex8_sorted_word_list(const string& infile) {
    ifstream in(infile);
    if (!in) { cerr << "ex8: cannot open " << infile << "\n"; return {}; }
    ostringstream buf;
    buf << in.rdbuf();
    string cleaned = ex7_expand_and_clean(buf.str());

    istringstream iss(cleaned);
    set<string> words;             // set gives us sorted + unique for free
    string w;
    while (iss >> w) if (!w.empty()) words.insert(w);
    return vector<string>(words.begin(), words.end());
}

// [9] 
vector<string> split(const string& s) {
    vector<string> result;
    istringstream iss(s);
    string tok;
    while (iss >> tok) result.push_back(tok);
    return result;
}

// [10] 
vector<string> split(const string& s, const string& w) {
    auto is_sep = [&](char c) {
        return isspace(static_cast<unsigned char>(c)) || w.find(c) != string::npos;
    };
    vector<string> result;
    string cur;
    for (char c : s) {
        if (is_sep(c)) {
            if (!cur.empty()) { result.push_back(cur); cur.clear(); }
        } else {
            cur += c;
        }
    }
    if (!cur.empty()) result.push_back(cur);
    return result;
}

// [11] 
void ex11_reverse_chars(const string& infile, const string& outfile) {
    ifstream in(infile);
    if (!in) { cerr << "ex11: cannot open " << infile << "\n"; return; }
    ostringstream buf; buf << in.rdbuf();
    string content = buf.str();
    reverse(content.begin(), content.end());
    ofstream out(outfile);
    out << content;
}

// [12] 
void ex12_reverse_words(const string& infile, const string& outfile) {
    ifstream in(infile);
    if (!in) { cerr << "ex12: cannot open " << infile << "\n"; return; }
    ostringstream buf; buf << in.rdbuf();
    vector<string> words = split(buf.str());
    reverse(words.begin(), words.end());
    ofstream out(outfile);
    for (size_t i = 0; i < words.size(); ++i) {
        if (i) out << ' ';
        out << words[i];
    }
}

// [13] 
void ex13_count_classifications(const string& infile) {
    ifstream in(infile);
    if (!in) { cerr << "ex13: cannot open " << infile << "\n"; return; }
    long alpha = 0, digit = 0, space = 0, punct = 0, upper = 0, lower = 0, other = 0;
    char c;
    while (in.get(c)) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (isalpha(uc)) ++alpha;
        if (isdigit(uc)) ++digit;
        if (isspace(uc)) ++space;
        if (ispunct(uc)) ++punct;
        if (isupper(uc)) ++upper;
        if (islower(uc)) ++lower;
        if (!isalpha(uc) && !isdigit(uc) && !isspace(uc) && !ispunct(uc)) ++other;
    }
    cout << "alpha=" << alpha << " digit=" << digit << " space=" << space
         << " punct=" << punct << " upper=" << upper << " lower=" << lower
         << " other=" << other << "\n";
}

// [14] 
void ex14_scientific_columns(const string& infile, const string& outfile) {
    ifstream in(infile);
    if (!in) { cerr << "ex14: cannot open " << infile << "\n"; return; }
    ofstream out(outfile);
    out << scientific << setprecision(8);
    double x;
    int col = 0;
    while (in >> x) {
        out << setw(20) << x;
        if (++col == 4) { out << '\n'; col = 0; }
    }
    if (col != 0) out << '\n';
}

// [15] 
void ex15_sorted_unique_counts(const string& infile) {
    ifstream in(infile);
    if (!in) { cerr << "ex15: cannot open " << infile << "\n"; return; }
    map<double, int> counts;           // map keeps keys sorted automatically
    double x;
    while (in >> x) ++counts[x];
    for (auto& [value, count] : counts) {
        cout << value;
        if (count > 1) cout << ' ' << count;
        cout << '\n';
    }
}

// [16] 
double ex16_sum(const string& infile) {
    ifstream in(infile);
    if (!in) { cerr << "ex16: cannot open " << infile << "\n"; return 0; }
    double x, total = 0;
    while (in >> x) total += x;
    return total;
}

// [17]-[19] 
struct Reading {
    string date;   
    string time;   
    double temp;   
};

// [17] store_temps.cpp: create raw_temps.txt with >= 50 readings.
void ex17_store_temps(const string& outfile = "raw_temps.txt", int count = 50) {
    ofstream out(outfile);
    // Synthetic but deterministic data so results are reproducible at defense time.
    for (int i = 0; i < count; ++i) {
        int day = 1 + (i % 28);
        int hour = i % 24;
        double temp = -5.0 + (i % 40) * 0.75;   // spread of plausible values
        out << "2024-01-" << setw(2) << setfill('0') << day << ' '
            << setw(2) << setfill('0') << hour << ":00 "
            << setfill(' ') << fixed << setprecision(2) << temp << '\n';
    }
}

// [18] 
vector<Reading> read_readings(const string& infile) {
    ifstream in(infile);
    vector<Reading> v;
    Reading r;
    while (in >> r.date >> r.time >> r.temp) v.push_back(r);
    return v;
}

void ex18_temp_stats(const string& infile = "raw_temps.txt") {
    vector<Reading> v = read_readings(infile);
    if (v.empty()) { cerr << "ex18: no data in " << infile << "\n"; return; }
    double sum = 0;
    for (auto& r : v) sum += r.temp;
    double mean = sum / v.size();

    vector<double> temps;
    for (auto& r : v) temps.push_back(r.temp);
    sort(temps.begin(), temps.end());
    double median = (temps.size() % 2)
                        ? temps[temps.size() / 2]
                        : (temps[temps.size() / 2 - 1] + temps[temps.size() / 2]) / 2.0;

    cout << "ex18: n=" << v.size() << " mean=" << mean << " median=" << median << "\n";
}

// [19] 
void ex19_store_temps_suffixed(const string& outfile = "raw_temps_suffixed.txt", int count = 50) {
    ofstream out(outfile);
    for (int i = 0; i < count; ++i) {
        int day = 1 + (i % 28);
        int hour = i % 24;
        double temp = -5.0 + (i % 40) * 0.75;
        char suffix = (i % 2 == 0) ? 'C' : 'F';
        out << "2024-01-" << setw(2) << setfill('0') << day << ' '
            << setw(2) << setfill('0') << hour << ":00 "
            << setfill(' ') << fixed << setprecision(2) << temp << suffix << '\n';
    }
}

void ex19_temp_stats_fahrenheit(const string& infile = "raw_temps_suffixed.txt") {
    ifstream in(infile);
    if (!in) { cerr << "ex19: cannot open " << infile << "\n"; return; }
    string date, time, tempstr;
    vector<double> f_temps;
    while (in >> date >> time >> tempstr) {
        char suffix = tempstr.back();
        double val = stod(tempstr.substr(0, tempstr.size() - 1));
        double f = (suffix == 'C') ? val * 9.0 / 5.0 + 32.0 : val;   
        f_temps.push_back(f);
    }
    if (f_temps.empty()) { cerr << "ex19: no data\n"; return; }
    double sum = accumulate(f_temps.begin(), f_temps.end(), 0.0);
    cout << "ex19: mean(F)=" << sum / f_temps.size() << "\n";
}

// [20] 
void print_year(int year) {
    cout << setw(4) << setfill('0') << year << setfill(' ');
}

// [21] 
class Roman_int {
    int val = 0;
public:
    Roman_int() = default;
    Roman_int(int v) : val(v) {}
    int as_int() const { return val; }

    friend ostream& operator<<(ostream& os, const Roman_int& r);
    friend istream& operator>>(istream& is, Roman_int& r);
};

// int --> Roman numeral string
string int_to_roman(int n) {
    if (n <= 0) throw runtime_error("Roman numerals only represent positive integers");
    static const vector<pair<int, string>> table = {
        {1000,"M"},{900,"CM"},{500,"D"},{400,"CD"},{100,"C"},{90,"XC"},
        {50,"L"},{40,"XL"},{10,"X"},{9,"IX"},{5,"V"},{4,"IV"},{1,"I"}
    };
    string result;
    for (auto& [value, symbol] : table)
        while (n >= value) { result += symbol; n -= value; }
    return result;
}

// Roman numeral string  int standard subtractive-pair scan
int roman_to_int(const string& s) {
    map<char, int> digit = {{'I',1},{'V',5},{'X',10},{'L',50},{'C',100},{'D',500},{'M',1000}};
    int total = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        int cur = digit.at(s[i]);
        int next = (i + 1 < s.size()) ? digit.at(s[i + 1]) : 0;
        total += (cur < next) ? -cur : cur;   // e.g. IV: I(1) < V(5) so subtract
    }
    return total;
}

ostream& operator<<(ostream& os, const Roman_int& r) { return os << int_to_roman(r.val); }
istream& operator>>(istream& is, Roman_int& r) {
    string s;
    is >> s;
    if (is) r.val = roman_to_int(s);
    return is;
}

void ex21_roman_int_demo() {
    Roman_int r(1994);
    cout << "Roman " << r << " equals " << r.as_int() << '\n';
    istringstream iss("MCMXCIX");
    Roman_int r2;
    iss >> r2;
    cout << "Roman " << r2 << " equals " << r2.as_int() << '\n';
}

// [22]
vector<string> roman_tokenize(const string& s) {
    vector<string> toks;
    string cur;
    for (char c : s) {
        if (isspace(static_cast<unsigned char>(c))) {
            if (!cur.empty()) { toks.push_back(cur); cur.clear(); }
        } else if (string("+-*/()").find(c) != string::npos) {
            if (!cur.empty()) { toks.push_back(cur); cur.clear(); }
            toks.push_back(string(1, c));
        } else {
            cur += c;
        }
    }
    if (!cur.empty()) toks.push_back(cur);
    return toks;
}

struct Roman_calc {
    vector<string> toks;
    size_t pos = 0;

    string peek() { return pos < toks.size() ? toks[pos] : ""; }
    string next() { return toks[pos++]; }

    int primary() {
        string t = next();
        if (t == "(") { int v = expr(); next(); /* ")" */ return v; }
        return roman_to_int(t);
    }
    int term() {
        int left = primary();
        while (peek() == "*" || peek() == "/") {
            string op = next();
            int right = primary();
            left = (op == "*") ? left * right : left / right;
        }
        return left;
    }
    int expr() {
        int left = term();
        while (peek() == "+" || peek() == "-") {
            string op = next();
            int right = term();
            left = (op == "+") ? left + right : left - right;
        }
        return left;
    }
};

int eval_roman_expr(const string& s) {
    Roman_calc calc;
    calc.toks = roman_tokenize(s);
    return calc.expr();
}

void ex22_roman_calculator_demo() {
    string expr = "XXI + CIV";
    int result = eval_roman_expr(expr);
    cout << expr << " == " << int_to_roman(result) << " (" << result << ")\n";
}

// [23] 
void ex23_concatenate(const string& file1, const string& file2, const string& outfile) {
    ifstream in1(file1), in2(file2);
    if (!in1) { cerr << "ex23: cannot open " << file1 << "\n"; return; }
    if (!in2) { cerr << "ex23: cannot open " << file2 << "\n"; return; }
    ofstream out(outfile);
    out << in1.rdbuf() << in2.rdbuf();
}

void make_sample_file(const string& name) {
    ofstream out(name);
    out << "Once upon a time!\n"
        << "\"Don't use the as-if rule.\" she said, sternly; then smiled.\n"
        << "The quick brown fox jumps over the lazy dog, 12345 times.\n";
}

// main(): menu-driven demo of every exercise.
int main() {
    make_sample_file("sample.txt");

    cout << "=== [1] lowercase ===\n";
    ex1_to_lowercase("sample.txt", "ex1_out.txt");
    { ifstream f("ex1_out.txt"); cout << f.rdbuf(); }

    cout << "\n=== [2] find word 'the' with line numbers ===\n";
    ex2_find_word("sample.txt", "the");

    cout << "\n=== [3] disemvowel ===\n";
    ex3_disemvowel("sample.txt", "ex3_out.txt");
    { ifstream f("ex3_out.txt"); cout << f.rdbuf(); }

    cout << "\n=== [4] multi_input (demo: 0x43 0123 65) ===\n";
    ex4_multi_input_demo();

    cout << "\n=== [5] character classification (demo) ===\n";
    ex5_classify_demo();

    cout << "\n=== [6] replace punctuation with whitespace ===\n";
    cout << ex6_replace_punct("\"- don't use the as-if rule.\" she said.") << "\n";

    cout << "\n=== [7] expand contractions + clean + lowercase ===\n";
    cout << ex7_expand_and_clean("Don't use the as-if rule.") << "\n";

    cout << "\n=== [8] sorted word list from sample.txt ===\n";
    for (auto& w : ex8_sorted_word_list("sample.txt")) cout << w << ' ';
    cout << "\n";

    cout << "\n=== [9]/[10] split() ===\n";
    for (auto& w : split("  hello   world  foo ")) cout << '[' << w << ']';
    cout << "\n";
    for (auto& w : split("a,b;c d", ",;")) cout << '[' << w << ']';
    cout << "\n";

    cout << "\n=== [11] reverse characters ===\n";
    { ofstream("rev_in.txt") << "asdfghjkl"; }
    ex11_reverse_chars("rev_in.txt", "rev_out.txt");
    { ifstream f("rev_out.txt"); cout << f.rdbuf() << "\n"; }

    cout << "\n=== [12] reverse words ===\n";
    { ofstream("rev_words_in.txt") << "Norwegian Blue parrot"; }
    ex12_reverse_words("rev_words_in.txt", "rev_words_out.txt");
    { ifstream f("rev_words_out.txt"); cout << f.rdbuf() << "\n"; }

    cout << "\n=== [13] classification counts ===\n";
    ex13_count_classifications("sample.txt");

    cout << "\n=== [14] scientific columns ===\n";
    { ofstream f("nums.txt"); f << "1 2 3 4 5 6 7 8 9"; }
    ex14_scientific_columns("nums.txt", "sci_out.txt");
    { ifstream f("sci_out.txt"); cout << f.rdbuf(); }

    cout << "\n=== [15] sorted unique + counts (75573 117 5 5 5) ===\n";
    { ofstream f("nums2.txt"); f << "75573 117 5 5 5"; }
    ex15_sorted_unique_counts("nums2.txt");

    cout << "\n=== [16] sum ===\n";
    cout << ex16_sum("nums2.txt") << "\n";

    cout << "\n=== [17]/[18]/[19] temperature readings ===\n";
    ex17_store_temps();
    ex18_temp_stats();
    ex19_store_temps_suffixed();
    ex19_temp_stats_fahrenheit();

    cout << "\n=== [20] print_year ===\n";
    print_year(7); cout << "\n";

    cout << "\n=== [21] Roman_int ===\n";
    ex21_roman_int_demo();

    cout << "\n=== [22] Roman-numeral calculator ===\n";
    ex22_roman_calculator_demo();

    cout << "\n=== [23] concatenate two files ===\n";
    { ofstream("f1.txt") << "Hello, "; ofstream("f2.txt") << "world!"; }
    ex23_concatenate("f1.txt", "f2.txt", "f3.txt");
    { ifstream f("f3.txt"); cout << f.rdbuf() << "\n"; }

    return 0;
}