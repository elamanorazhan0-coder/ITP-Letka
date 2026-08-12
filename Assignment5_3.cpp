#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// [1] 
void to_lower(char* s) {
    // A C-style string is zero-terminated, so we walk until we hit
    // that terminating 0 byte -- that IS the "end of string" test.
    while (*s != 0) {
        if (*s >= 'A' && *s <= 'Z')
            *s = *s - 'A' + 'a';
        ++s;
    }
}

// [2] str_dup copy a C-style string into free-store
char* str_dup(const char* s) {
    // First pass find the length by counting up to the terminator,
    // since we need to know how much to allocate.
    int len = 0;
    while (s[len] != 0) ++len;

    char* copy = new char[len + 1];    // +1 for the terminating 0
    for (int i = 0; i <= len; ++i)     // <= so the 0 terminator is copied too
        copy[i] = s[i];
    return copy;
}

// [3] 
char* find_x(const char* s, const char* x) {
    for (; *s; ++s) {
        const char* p = s;
        const char* q = x;
        while (*q && *p == *q) { ++p; ++q; }
        if (*q == 0)                       
            return const_cast<char*>(s);
    }
    return nullptr;
}

// [4] 
int str_cmp(const char* s1, const char* s2) {
    while (*s1 != 0 && *s1 == *s2) { ++s1; ++s2; }
    // Either both ended together (equal), or they differ at *s1/*s2.
    return static_cast<unsigned char>(*s1) - static_cast<unsigned char>(*s2);
}

// [5] 
void ex5_bad_pointer_demo() {
    cout << "ex5: WITHOUT a terminating 0, str_cmp/find_x/str_dup have no\n"
         << "     way to know where the string ends, so they keep reading\n"
         << "     (and str_dup keeps writing) past the buffer -- undefined\n"
         << "     behavior. We do NOT actually invoke that here (it can\n"
         << "     crash or corrupt memory unpredictably); the safe versions\n"
         << "     below are the fix.\n";
}

char* str_dup_safe(const char* s, int maxlen) {
    int len = 0;
    while (len < maxlen && s[len] != 0) ++len;
    char* copy = new char[len + 1];
    for (int i = 0; i < len; ++i) copy[i] = s[i];
    copy[len] = 0;                      // we terminate the copy ourselves
    return copy;
}

char* find_x_safe(const char* s, const char* x, int maxlen) {
    int slen = 0; while (slen < maxlen && s[slen] != 0) ++slen;
    int xlen = 0; while (x[xlen] != 0) ++xlen;   // x is assumed to be a real C-string
    for (int i = 0; i + xlen <= slen; ++i) {
        int j = 0;
        while (j < xlen && s[i + j] == x[j]) ++j;
        if (j == xlen) return const_cast<char*>(s + i);
    }
    return nullptr;
}

int str_cmp_safe(const char* s1, const char* s2, int maxlen) {
    int i = 0;
    while (i < maxlen && s1[i] != 0 && s1[i] == s2[i]) ++i;
    if (i == maxlen) return 0;          // treated as "equal within the bound"
    return static_cast<unsigned char>(s1[i]) - static_cast<unsigned char>(s2[i]);
}

void ex5_safe_versions_demo() {
    
    char* fake = new char[5];
    fake[0]='h'; fake[1]='e'; fake[2]='l'; fake[3]='l'; fake[4]='o'; // no 0 !

    char* d = str_dup_safe(fake, 5);
    cout << "ex5: str_dup_safe on unterminated buffer -> \"" << d << "\"\n";
    delete[] d;
    delete[] fake;

    char stack_fake[3] = {'h','i','!'};   // also unterminated
    cout << "ex5: str_cmp_safe(\"hi!\", \"hi?\", 3) = "
         << str_cmp_safe(stack_fake, "hi?", 3) << " (bound stops the read at 3)\n";
}

void ex6_strcmp_bad_pointer_discussion() {
    cout << "ex6: std::strcmp has exactly the same problem as our hand-written\n"
         << "     str_cmp: it also just walks memory looking for a 0 byte, so\n"
         << "     handing it a non-terminated buffer is undefined behavior --\n"
         << "     it may read arbitrarily far past the buffer until it happens\n"
         << "     to find a 0 somewhere in memory, at best returning garbage\n"
         << "     and at worst crashing (segfault) if it walks off a mapped page.\n";
}

string cat_dot(const char* s1, const char* s2) {
    string result;
    for (const char* p = s1; *p; ++p) result += *p;
    result += '.';
    for (const char* p = s2; *p; ++p) result += *p;
    return result;
}

string cat_dot(const string& s1, const string& s2) {
    return s1 + '.' + s2;
}

string cat_dot(const string& s1, const string& s2, const string& sep) {
    return s1 + sep + s2;
}

char* cat_dot_cstyle(const char* s1, const char* s2) {
    int len1 = 0; while (s1[len1]) ++len1;
    int len2 = 0; while (s2[len2]) ++len2;
    char* result = new char[len1 + 1 + len2 + 1];   // s1 + '.' + s2 + '\0'
    int k = 0;
    for (int i = 0; i < len1; ++i) result[k++] = s1[i];
    result[k++] = '.';
    for (int i = 0; i < len2; ++i) result[k++] = s2[i];
    result[k] = 0;
    return result;
}

char* cat_sep_cstyle(const char* s1, const char* s2, const char* sep) {
    int len1 = 0; while (s1[len1]) ++len1;
    int len2 = 0; while (s2[len2]) ++len2;
    int lensep = 0; while (sep[lensep]) ++lensep;
    char* result = new char[len1 + lensep + len2 + 1];
    int k = 0;
    for (int i = 0; i < len1; ++i) result[k++] = s1[i];
    for (int i = 0; i < lensep; ++i) result[k++] = sep[i];
    for (int i = 0; i < len2; ++i) result[k++] = s2[i];
    result[k] = 0;
    return result;
}

void ex10_cstyle_cat_demo() {
    char* a = cat_dot_cstyle("Niels", "Bohr");
    cout << "ex10: cat_dot_cstyle -> " << a << "\n";
    delete[] a;

    char* b = cat_sep_cstyle("2024", "01", " / ");
    cout << "ex10: cat_sep_cstyle -> " << b << "\n";
    delete[] b;
}

char* reverse_copy(const char* s) {
    int len = 0; while (s[len]) ++len;
    char* rev = new char[len + 1];
    for (int i = 0; i < len; ++i) rev[i] = s[len - 1 - i];
    rev[len] = 0;
    return rev;
}

bool is_palindrome(const char* s) {
    char* rev = reverse_copy(s);
    bool result = (str_cmp(s, rev) == 0);
    delete[] rev;
    return result;
}

void ex11_palindrome_demo() {
    for (const char* word : {"home", "level", "racecar", "Stroustrup"}) {
        char* rev = reverse_copy(word);
        cout << "ex11: \"" << word << "\" reversed is \"" << rev << "\" -> "
             << (is_palindrome(word) ? "palindrome" : "not a palindrome") << "\n";
        delete[] rev;
    }
}

// main(): menu-driven demo of every exercise.
int main() {
    cout << "=== [1] to_lower ===\n";
    { char s[] = "Hello, World!"; to_lower(s); cout << s << "\n"; }

    cout << "\n=== [2] str_dup ===\n";
    { char* d = str_dup("Astana"); cout << d << "\n"; delete[] d; }

    cout << "\n=== [3] find_x ===\n";
    { char* p = find_x("the quick brown fox", "quick"); cout << (p ? p : "(not found)") << "\n"; }

    cout << "\n=== [4] str_cmp ===\n";
    cout << "str_cmp(\"abc\",\"abd\") = " << str_cmp("abc", "abd") << "\n"
         << "str_cmp(\"abc\",\"abc\") = " << str_cmp("abc", "abc") << "\n"
         << "str_cmp(\"abd\",\"abc\") = " << str_cmp("abd", "abc") << "\n";

    cout << "\n=== [5] bad pointers + safe redesign ===\n";
    ex5_bad_pointer_demo();
    ex5_safe_versions_demo();

    cout << "\n=== [6] strcmp with a bad pointer (discussion) ===\n";
    ex6_strcmp_bad_pointer_discussion();

    cout << "\n=== [7] cat_dot(const char*, const char*) ===\n";
    cout << cat_dot("Niels", "Bohr") << "\n";

    cout << "\n=== [8] cat_dot(const string&, const string&) ===\n";
    cout << cat_dot(string("Niels"), string("Bohr")) << "\n";

    cout << "\n=== [9] cat_dot with separator argument ===\n";
    cout << cat_dot(string("Niels"), string("Bohr"), string(" - ")) << "\n";

    cout << "\n=== [10] cat_dot returning a free-store C-string ===\n";
    ex10_cstyle_cat_demo();

    cout << "\n=== [11] palindrome check via backward copy ===\n";
    ex11_palindrome_demo();

    return 0;
}