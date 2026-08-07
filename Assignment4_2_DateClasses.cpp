// Exercises 10, 11, 12 Date class
#include <iostream>
#include <stdexcept>
using namespace std;

// [10] leapyear(int)
bool leapyear(int y) {
    // standard Gregorian rule: divisible by 4, except centuries,
    // unless also divisible by 400
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

// Small helpers shared by both Date designs below - a correct, well-known algorithm (civil calendar <-> day count) so we
// don't have to hand-roll fragile date math twice.
// Reference: Howard Hinnant's "chrono-compatible low-level date 
//algorithms" - it's the standard trick used internally by std::chrono::year_month_day in C++20.
namespace DateMath {

// days since 1970-01-01 (can be negative for earlier dates)
long days_from_civil(int y, unsigned m, unsigned d) {
    y -= m <= 2;
    long era = (y >= 0 ? y : y - 399) / 400;
    unsigned yoe = static_cast<unsigned>(y - era * 400);         // [0, 399]
    unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1; // [0, 365]
    unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;          // [0, 146096]
    return era * 146097 + static_cast<long>(doe) - 719468;
}

void civil_from_days(long z, int &y, unsigned &m, unsigned &d) {
    z += 719468;
    long era = (z >= 0 ? z : z - 146096) / 146097;
    unsigned doe = static_cast<unsigned>(z - era * 146097);            // [0, 146096]
    unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
    y = static_cast<int>(yoe) + era * 400;
    unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);   // [0, 365]
    unsigned mp = (5 * doy + 2) / 153;                        // [0, 11]
    d = doy - (153 * mp + 2) / 5 + 1;                         // [1, 31]
    m = mp + (mp < 10 ? 3 : -9);                              // [1, 12]
    y += (m <= 2);
}

// 1970-01-01 was a Thursday. day_count % 7 tells us how far from
// that we are; 0=Sunday .. 6=Saturday after the offset below.
int weekday(long days) {
    long wd = (days + 4) % 7;   // +4 shifts Thursday(index0) to land on Sunday=0
    if (wd < 0) wd += 7;
    return (int)wd;
}

} // namespace DateMath

// 11 Date class (year/month/day representation) with helper functions next_workday() and week_of_year()
namespace Ex11 {

class Date {
    int y;
    unsigned m, d;
public:
    Date(int year, unsigned month, unsigned day) : y(year), m(month), d(day) {
        if (month < 1 || month > 12 || day < 1 || day > 31)
            throw invalid_argument("Bad date");
    }
    int year() const { return y; }
    unsigned month() const { return m; }
    unsigned day() const { return d; }

    long to_days() const { return DateMath::days_from_civil(y, m, d); }

    // [11] next_workday(): any day that isn't Sat/Sun
    Date next_workday() const {
        long days = to_days() + 1;
        while (DateMath::weekday(days) == 0 || DateMath::weekday(days) == 6)
            days++;   // 0=Sunday, 6=Saturday -> keep skipping weekend days
        int ny; unsigned nm, nd;
        DateMath::civil_from_days(days, ny, nm, nd);
        return Date(ny, nm, nd);
    }

    // [11] week_of_year(): week 1 = the week containing Jan 1,
    // weeks start on Sunday
    int week_of_year() const {
        long jan1 = DateMath::days_from_civil(y, 1, 1);
        int jan1_weekday = DateMath::weekday(jan1);   // 0=Sunday..6=Saturday
        long days_into_year = to_days() - jan1;
        // shift so the partial first week still counts as week 1
        return (int)((days_into_year + jan1_weekday) / 7) + 1;
    }

    void print() const {
        cout << y << "-" << m << "-" << d;
    }
};

void demo() {
    cout << "\n--- [10]/[11] leapyear(), next_workday(), week_of_year() ---\n";
    cout << "leapyear(2024) = " << boolalpha << leapyear(2024) << "\n";
    cout << "leapyear(2023) = " << leapyear(2023) << "\n";
    cout << "leapyear(1900) = " << leapyear(1900) << " (century, not /400)\n";
    cout << "leapyear(2000) = " << leapyear(2000) << " (century, but /400)\n";

    Date d(2026, 8, 7);   // Friday, per the conversation's "current date"
    cout << "Date: "; d.print(); cout << "\n";

    Date nw = d.next_workday();
    cout << "Next workday: "; nw.print(); cout << "\n";

    cout << "Week of year: " << d.week_of_year() << "\n";
}

} // namespace Ex11

// [12] Date re-implemented with day-count-since-1970 storage
namespace Ex12 {

class Date {
    long days;   // days since 1970-01-01 (day 0); negative days rejected
public:
    // build from a calendar date
    Date(int year, unsigned month, unsigned day) {
        long d = DateMath::days_from_civil(year, month, day);
        if (d < 0) throw invalid_argument("Date before 1970-01-01 not supported");
        days = d;
    }
    // build directly from a day count (also useful internally)
    explicit Date(long dayCount) {
        if (dayCount < 0) throw invalid_argument("Negative day count not allowed");
        days = dayCount;
    }

    int year() const { int y; unsigned m, d; DateMath::civil_from_days(days, y, m, d); return y; }
    unsigned month() const { int y; unsigned m, d; DateMath::civil_from_days(days, y, m, d); return m; }
    unsigned day() const { int y; unsigned m, d; DateMath::civil_from_days(days, y, m, d); return d; }
    long day_count() const { return days; }

    Date add_day(int n) const { return Date(days + n); }   // trivial now - just add to the count

    bool operator==(const Date &o) const { return days == o.days; }
    bool operator!=(const Date &o) const { return !(*this == o); }
    bool operator<(const Date &o) const { return days < o.days; }

    void print() const {
        int y; unsigned m, d;
        DateMath::civil_from_days(days, y, m, d);
        cout << y << "-" << m << "-" << d << " (day " << days << ")";
    }
};

void demo() {
    cout << "\n--- [12] Date reimplemented as day-count since 1970-01-01 ---\n";
    Date d(2026, 8, 7);
    cout << "Date: "; d.print(); cout << "\n";

    Date tomorrow = d.add_day(1);
    cout << "add_day(1): "; tomorrow.print(); cout << "\n";

    cout << "d == tomorrow ? " << boolalpha << (d == tomorrow) << "\n";
    cout << "d < tomorrow ?  " << (d < tomorrow) << "\n";

    // negative days rejected, as required
    try {
        Date bad(1969, 12, 31);
    } catch (const invalid_argument &e) {
        cout << "Rejected date before day 0: " << e.what() << "\n";
    }
}

} // namespace Ex12

// ---------------------------------------------------------
int main() {
    Ex11::demo();
    Ex12::demo();
    return 0;
}