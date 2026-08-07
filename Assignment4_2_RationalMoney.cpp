// Exercises 13, 14, 15, 16, 17, 18 Rational and Money
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <numeric>   // std::gcd
#include <stdexcept>
using namespace std;

// [13] Rational number class
namespace Ex13 {
long gcd(long a, long b) {
    while (b != 0) {
        long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

class Rational {
    long num, den;   // numerator / denominator, always kept in lowest terms

    void reduce() {
        if (den < 0) { num = -num; den = -den; }   // keep sign on numerator only
        long g = gcd(num < 0 ? -num : num, den);
        if (g != 0) { num /= g; den /= g; }
    }
public:
    Rational(long n = 0, long d = 1) : num(n), den(d) {
        if (d == 0) throw invalid_argument("Zero denominator");
        reduce();
    }

    Rational& operator=(const Rational &o) { num = o.num; den = o.den; return *this; }

    Rational operator+(const Rational &o) const { return Rational(num * o.den + o.num * den, den * o.den); }
    Rational operator-(const Rational &o) const { return Rational(num * o.den - o.num * den, den * o.den); }
    Rational operator*(const Rational &o) const { return Rational(num * o.num, den * o.den); }
    Rational operator/(const Rational &o) const {
        if (o.num == 0) throw invalid_argument("Division by zero");
        return Rational(num * o.den, den * o.num);
    }
    bool operator==(const Rational &o) const { return num == o.num && den == o.den; }   // already reduced, so this is safe

    double to_double() const { return (double)num / (double)den; }

    friend ostream& operator<<(ostream &os, const Rational &r) {
        return os << r.num << "/" << r.den;
    }
};

void demo() {
    cout << "\n--- [13] Rational class ---\n";
    Rational a(5, 6), b(1, 3);
    cout << "a = " << a << ", b = " << b << "\n";
    cout << "a + b = " << (a + b) << "\n";
    cout << "a - b = " << (a - b) << "\n";
    cout << "a * b = " << (a * b) << "\n";
    cout << "a / b = " << (a / b) << "\n";
    cout << "a == a ? " << boolalpha << (a == a) << "\n";
    cout << "a as double: " << a.to_double() << "\n";
    cout << "\nWhy use Rational? Because it keeps EXACT fractions (5/6 stays\n"
         << "exactly 5/6) instead of rounding to the nearest double, which\n"
         << "matters for things like currency splits or repeated calculations\n"
         << "where rounding errors would otherwise pile up.\n";
}

} // namespace Ex13

// [14] Money class: whole cents stored as a long int, with 4/5 rounding on construction from a dollar amount

namespace Ex14 {

class Money {
    long cents;
public:
    // build straight from a dollar amount, e.g. Money(123.45)
    Money(double dollars = 0.0) {
        // 4/5 rounding: .5 rounds up, anything less rounds down.
        // round() already does exactly that for positive values.
        cents = (long)round(dollars * 100.0);
    }

    double dollars() const { return cents / 100.0; }
    long total_cents() const { return cents; }

    Money operator+(const Money &o) const { Money m; m.cents = cents + o.cents; return m; }
    Money operator-(const Money &o) const { Money m; m.cents = cents - o.cents; return m; }

    friend ostream& operator<<(ostream &os, const Money &m) {
        long c = m.cents;
        bool neg = c < 0;
        if (neg) c = -c;
        os << (neg ? "-$" : "$") << (c / 100) << "." ;
        if (c % 100 < 10) os << "0";
        os << (c % 100);
        return os;
    }
};

void demo() {
    cout << "\n--- [14] Money class (cents, 4/5 rounding) ---\n";
    Money a(123.45);
    Money b(0.005);     // exactly half a cent -> rounds up per 4/5 rule
    Money c(0.004);     // less than half a cent -> rounds down

    cout << "a = " << a << " (" << a.total_cents() << " cents)\n";
    cout << "b (from 0.005) = " << b << " (" << b.total_cents() << " cents)\n";
    cout << "c (from 0.004) = " << c << " (" << c.total_cents() << " cents)\n";
    cout << "a + b = " << (a + b) << "\n";
}

} // namespace Ex14

// [15] [16] Refined Money: currency-aware, input/output operators for things like "USD1.23"
namespace Ex15_16 {

// simple fixed conversion table relative to USD (for demo purposes)
map<string, double> conversionToUSD = {
    {"USD", 1.0},
    {"DKK", 0.145}   // roughly, just an example rate
};

class Money {
    long cents;
    string currency;

    // internal-only: build directly from a cent count, no validation.
    // Needed because computed sums (e.g. currency conversion) can end
    // up with tiny floating-point noise that shouldn't trigger the strict "must be exact" check meant for user-entered literals.
    Money(long c, string curr, bool) : cents(c), currency(curr) {}
public:
    Money(double amount = 0.0, string curr = "USD") : currency(curr) {
        // [15]: only accept the value if amount*100 is (very close to)
        // a whole number - i.e. it can be exactly represented in cents
        double c = amount * 100.0;
        double rounded = round(c);
        if (abs(c - rounded) > 1e-6)
            throw invalid_argument("Amount cannot be represented exactly in cents");
        cents = (long)rounded;
    }

    double dollars() const { return cents / 100.0; }
    string get_currency() const { return currency; }

    // [15]: Money*Money is intentionally NOT defined - multiplying
    // two money amounts together doesn't mean anything ($5 * $3 isn't "$15", it would be "15 square dollars", which is meaningless)

    // addition only makes sense directly for same currency; different
    // currencies need the conversion table first
    Money operator+(const Money &o) const {
        if (currency == o.currency)
            return Money(cents + o.cents, currency, true);   // exact - just add cent counts

        // convert both sides to USD, add, then express result in USD
        if (!conversionToUSD.count(currency) || !conversionToUSD.count(o.currency))
            throw invalid_argument("No conversion rate available for these currencies");
        double usdTotal = dollars() * conversionToUSD[currency] + o.dollars() * conversionToUSD[o.currency];
        return Money((long)round(usdTotal * 100.0), "USD", true);   // round to nearest cent, don't re-validate
    }

    friend ostream& operator<<(ostream &os, const Money &m);
    friend istream& operator>>(istream &is, Money &m);
};

// [16] output like "USD1.23"
ostream& operator<<(ostream &os, const Money &m) {
    long c = m.cents;
    os << m.currency << (c / 100) << "." << (c % 100 < 10 ? "0" : "") << (c % 100);
    return os;
}

// [16] input like "USD1.23" - reads currency code (3 letters) followed immediately by the numeric amount, no space between them
istream& operator>>(istream &is, Money &m) {
    string token;
    is >> token;
    if (token.size() < 4) { is.setstate(ios::failbit); return is; }

    string curr = token.substr(0, 3);
    double amount;
    try {
        amount = stod(token.substr(3));
    } catch (...) {
        is.setstate(ios::failbit);
        return is;
    }
    m = Money(amount, curr);
    return is;
}

void demo() {
    cout << "\n--- [15]/[16] Currency-aware Money, >> and << ---\n";
    Money usd(1.23, "USD");
    Money dkk(5.00, "DKK");
    cout << "usd = " << usd << ", dkk = " << dkk << "\n";

    Money sum = usd + dkk;   // uses the conversion table
    cout << "usd + dkk = " << sum << " (converted via table)\n";

    // parsing from a stream, like the exercise asks
    cout << "\nParsing \"USD1.23\" from a stream:\n";
    istringstream iss("USD1.23");
    Money parsed;
    iss >> parsed;
    cout << "Parsed: " << parsed << "\n";

    try {
        Money bad(1.005);   // not exactly representable in cents
    } catch (const invalid_argument &e) {
        cout << "\nRejected: " << e.what() << "\n";
    }
}

} // namespace Ex15_16

// [17] Rational beats Money: exact thirds
namespace Ex17 {
void demo() {
    cout << "\n--- [17] Rational vs Money example ---\n";
    // splitting $10 three ways: Money is stuck at whole cents,
    // so 3 x $3.33 = $9.99, one cent goes missing.
    Ex14::Money share(10.0 / 3.0);
    Ex14::Money threeShares = share + share + share;
    cout << "Money: $10 split 3 ways = " << share << " each, x3 = " << threeShares
         << " (a cent got lost to rounding!)\n";

    // Rational keeps it exact: 10/3 stays 10/3, no information lost.
    Ex13::Rational r(10, 3);
    Ex13::Rational total = r + r + r;
    cout << "Rational: 10/3 + 10/3 + 10/3 = " << total << " (exactly 10, no loss)\n";
}
} // namespace Ex17

// [18] Rational beats double: repeating fractions
namespace Ex18 {
void demo() {
    cout << "\n--- [18] Rational vs double example ---\n";
    // 1/3 three times is a bad example here - the rounding errors
    // happen to cancel out and double gives exactly 1.0. The classic,
    // reliable example is 0.1 + 0.2, which double can NOT store exactly (neither 0.1 nor 0.2 has an exact binary representation).
    double d = 0.1 + 0.2;
    cout << "double: 0.1 + 0.2 = " << d
         << " (compare to 0.3: equal? " << boolalpha << (d == 0.3) << ")\n";

    Ex13::Rational r = Ex13::Rational(1, 10) + Ex13::Rational(2, 10);
    cout << "Rational: 1/10 + 2/10 = " << r
         << " (compare to 3/10: equal? " << (r == Ex13::Rational(3, 10)) << ")\n";
    cout << "0.1 and 0.2 don't have exact binary floating-point representations,\n"
         << "so their sum drifts slightly away from 0.3. Rational stores the\n"
         << "fraction itself (numerator/denominator), so 1/10 + 2/10 lands on\n"
         << "exactly 3/10, no drift at all.\n";
}
} // namespace Ex18

// ---------------------------------------------------------
int main() {
    Ex13::demo();
    Ex14::demo();
    Ex15_16::demo();
    Ex17::demo();
    Ex18::demo();
    return 0;
}