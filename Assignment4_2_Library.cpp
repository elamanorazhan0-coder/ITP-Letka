// Exercises 5, 6, 7, 8, 9 Library system
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

// small self-contained Date (day-count based, same idea as exercise 12) - just enough for Transaction to have a date
class Date {
    int y; unsigned m, d;
public:
    Date(int year = 1970, unsigned month = 1, unsigned day = 1) : y(year), m(month), d(day) {}
    void print(ostream &os) const { os << y << "-" << m << "-" << d; }
};
ostream& operator<<(ostream &os, const Date &dt) { dt.print(os); return os; }

// [7] Genre enum, used by Book below
enum class Genre { fiction, nonfiction, periodical, biography, children };

ostream& operator<<(ostream &os, Genre g) {
    switch (g) {
        case Genre::fiction:     return os << "fiction";
        case Genre::nonfiction:  return os << "nonfiction";
        case Genre::periodical:  return os << "periodical";
        case Genre::biography:   return os << "biography";
        case Genre::children:    return os << "children";
    }
    return os;   // unreachable, but silences some compiler warnings
}

// [5] [6] [7] Book class
class Book {
    string isbn;
    string title;
    string author;
    int copyright_date;
    bool checked_out_flag;
    Genre genre;

    // [5] validate ISBN of the form n-n-n-x, where n is an
    // integer (one or more digits) and the last group x is a single digit or letter
    static bool validISBN(const string &s) {
        istringstream iss(s);
        string part;
        vector<string> parts;
        while (getline(iss, part, '-')) parts.push_back(part);
        if (parts.size() != 4) return false;

        for (int i = 0; i < 3; i++) {
            if (parts[i].empty()) return false;
            for (char c : parts[i])
                if (!isdigit((unsigned char)c)) return false;   // must be all digits
        }
        // last part: exactly one digit or letter
        if (parts[3].size() != 1) return false;
        char last = parts[3][0];
        return isdigit((unsigned char)last) || isalpha((unsigned char)last);
    }

public:
    Book(const string &isbn_, const string &title_, const string &author_,
         int copyright_, Genre g = Genre::fiction)
        : isbn(isbn_), title(title_), author(author_),
          copyright_date(copyright_), checked_out_flag(false), genre(g) {
        if (!validISBN(isbn))
            throw invalid_argument("Bad ISBN format: " + isbn);
    }

    // accessors, per [5]: "create functions for returning those data values"
    string get_isbn() const { return isbn; }
    string get_title() const { return title; }
    string get_author() const { return author; }
    int get_copyright_date() const { return copyright_date; }
    bool is_checked_out() const { return checked_out_flag; }
    Genre get_genre() const { return genre; }

    void check_out() { checked_out_flag = true; }
    void check_in()  { checked_out_flag = false; }

    // [6] operators
    friend bool operator==(const Book &a, const Book &b);
    friend bool operator!=(const Book &a, const Book &b);
    friend ostream& operator<<(ostream &os, const Book &b);
};

bool operator==(const Book &a, const Book &b) { return a.isbn == b.isbn; }
bool operator!=(const Book &a, const Book &b) { return !(a == b); }
ostream& operator<<(ostream &os, const Book &b) {
    os << b.title << "\n" << b.author << "\n" << b.isbn;
    return os;
}

// [8] Patron class
class Patron {
    string name;
    int card_number;
    double fee_owed;
public:
    Patron(const string &n, int card) : name(n), card_number(card), fee_owed(0.0) {}

    string get_name() const { return name; }
    int get_card_number() const { return card_number; }
    double get_fee() const { return fee_owed; }

    void set_fee(double amount) { fee_owed = amount; }

    // helper: does this patron currently owe anything?
    bool owes_fee() const { return fee_owed > 0.0; }
};

// [9] Library class: vectors of Book/Patron, Transaction struct
struct Transaction {
    Book book;
    Patron patron;
    Date date;
};

class Library {
    vector<Book> books;
    vector<Patron> patrons;
    vector<Transaction> transactions;

    // find helpers - return pointer so we can tell "not found" apart from a real match (nullptr means not found)
    Book* find_book(const string &isbn) {
        for (auto &b : books) if (b.get_isbn() == isbn) return &b;
        return nullptr;
    }
    Patron* find_patron(int card) {
        for (auto &p : patrons) if (p.get_card_number() == card) return &p;
        return nullptr;
    }

public:
    void add_book(const Book &b) { books.push_back(b); }
    void add_patron(const Patron &p) { patrons.push_back(p); }

    // checks out a book: both book and patron must be registered in the library, and the patron must not owe any fees
    bool check_out(const string &isbn, int card, const Date &today) {
        Book *b = find_book(isbn);
        Patron *p = find_patron(card);

        if (!b) { cout << "Error: book " << isbn << " is not in the library.\n"; return false; }
        if (!p) { cout << "Error: no patron with card " << card << ".\n"; return false; }
        if (p->owes_fee()) {
            cout << "Error: " << p->get_name() << " owes a fee and cannot check out books.\n";
            return false;
        }

        b->check_out();
        transactions.push_back({*b, *p, today});
        cout << p->get_name() << " checked out \"" << b->get_title() << "\"\n";
        return true;
    }

    // returns the names of every patron who currently owes a fee
    vector<string> patrons_who_owe() const {
        vector<string> result;
        for (const auto &p : patrons)
            if (p.owes_fee()) result.push_back(p.get_name());
        return result;
    }

    void print_transactions() const {
        cout << "Transactions so far:\n";
        for (const auto &t : transactions)
            cout << "  " << t.patron.get_name() << " <- \"" << t.book.get_title()
                 << "\" on " << t.date << "\n";
    }
};

// ---------------------------------------------------------
void demo() {
    cout << "\n--- [5]-[9] Library system ---\n";

    Library lib;
    lib.add_book(Book("0-13-110362-8", "The C Programming Language", "Kernighan & Ritchie", 1978, Genre::nonfiction));
    lib.add_book(Book("0-201-70073-5", "The C++ Programming Language", "Bjarne Stroustrup", 2000, Genre::nonfiction));

    Patron p1("Yelaman", 1001);
    Patron p2("Aigerim", 1002);
    p2.set_fee(5.50);   // Aigerim owes a fee
    lib.add_patron(p1);
    lib.add_patron(p2);

    cout << "\nAttempting checkouts:\n";
    lib.check_out("0-13-110362-8", 1001, Date(2026, 8, 7));   // should succeed
    lib.check_out("0-201-70073-5", 1002, Date(2026, 8, 7));   // should fail: fee owed
    lib.check_out("9-99-999999-9", 1001, Date(2026, 8, 7));   // should fail: unknown book

    cout << "\n";
    lib.print_transactions();

    cout << "\nPatrons who owe fees:\n";
    for (const auto &name : lib.patrons_who_owe()) cout << "  " << name << "\n";

    // [6] operator demo
    Book b1("0-13-110362-8", "The C Programming Language", "Kernighan & Ritchie", 1978);
    Book b2("0-13-110362-8", "Different Title", "Different Author", 1999);
    cout << "\nb1 == b2 (same ISBN, different everything else) ? "
         << boolalpha << (b1 == b2) << "\n";
    cout << "b1 printed via operator<<:\n" << b1 << "\n";

    // ISBN validation demo
    try {
        Book bad("123-abc", "Bad Book", "Nobody", 2020);
    } catch (const invalid_argument &e) {
        cout << "\nRejected book: " << e.what() << "\n";
    }
}

int main() {
    demo();
    return 0;
}