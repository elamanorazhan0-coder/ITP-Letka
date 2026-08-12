#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>

using namespace std;

class Matrix {
    int rows_, cols_;
    vector<double> elems;   // row-major: element (r,c) lives at r*cols_+c

    int index(int r, int c) const {
        if (r < 0 || r >= rows_ || c < 0 || c >= cols_)
            throw out_of_range("Matrix index out of range");
        return r * cols_ + c;
    }

public:
    //   since vector<double> already copies deeply)
    Matrix(int rows, int cols) : rows_(rows), cols_(cols), elems(rows * cols, 0.0) {
        if (rows <= 0 || cols <= 0) throw invalid_argument("Matrix dimensions must be positive");
    }
    Matrix(int rows, int cols, double init_value)
        : rows_(rows), cols_(cols), elems(rows * cols, init_value) {
        if (rows <= 0 || cols <= 0) throw invalid_argument("Matrix dimensions must be positive");
    }

    int rows() const { return rows_; }
    int cols() const { return cols_; }

    Matrix& operator=(const Matrix&) = default;

    bool operator==(const Matrix& other) const {
        return rows_ == other.rows_ && cols_ == other.cols_ && elems == other.elems;
    }
    bool operator!=(const Matrix& other) const { return !(*this == other); }

    double& operator()(int r, int c) { return elems[index(r, c)]; }
    const double& operator()(int r, int c) const { return elems[index(r, c)]; }

    // 1 operator+ (addition of corresponding elements)
    Matrix operator+(const Matrix& other) const {
        if (rows_ != other.rows_ || cols_ != other.cols_)
            throw invalid_argument("Matrix dimensions must match for +");
        Matrix result(rows_, cols_);
        for (size_t i = 0; i < elems.size(); ++i)
            result.elems[i] = elems[i] + other.elems[i];
        return result;
    }

    // [3] += : a natural companion to + that most users expect once +
    // exists. Implemented
    // in terms of + for now; a hand-optimized version could add in place without creating a temporary.
    Matrix& operator+=(const Matrix& other) {
        if (rows_ != other.rows_ || cols_ != other.cols_)
            throw invalid_argument("Matrix dimensions must match for +=");
        for (size_t i = 0; i < elems.size(); ++i)
            elems[i] += other.elems[i];
        return *this;
    }

    // [4] row(i) a copy of the i-th row as a vector<double>.
    vector<double> row(int i) const {
        if (i < 0 || i >= rows_) throw out_of_range("row index out of range");
        vector<double> result(cols_);
        for (int c = 0; c < cols_; ++c) result[c] = elems[index(i, c)];
        return result;
    }

    // [4] column(i) a copy of the i-th column as a vector<double>.
    vector<double> column(int i) const {
        if (i < 0 || i >= cols_) throw out_of_range("column index out of range");
        vector<double> result(rows_);
        for (int r = 0; r < rows_; ++r) result[r] = elems[index(r, i)];
        return result;
    }

    friend ostream& operator<<(ostream& os, const Matrix& m);
    friend istream& operator>>(istream& is, Matrix& m);
};

// 2 print the matrix, one row per line, values
// space-separated, dimensions on the first line so can read it back.
ostream& operator<<(ostream& os, const Matrix& m) {
    os << m.rows_ << ' ' << m.cols_ << '\n';
    for (int r = 0; r < m.rows_; ++r) {
        for (int c = 0; c < m.cols_; ++c) {
            os << m.elems[m.index(r, c)];
            if (c + 1 < m.cols_) os << ' ';
        }
        os << '\n';
    }
    return os;
}

// 2 operator read a matrix in the same format operator<< writes,
// so the two are round-trip compatible (read what you print).
istream& operator>>(istream& is, Matrix& m) {
    int r, c;
    if (!(is >> r >> c)) return is;
    Matrix tmp(r, c);
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j)
            if (!(is >> tmp.elems[tmp.index(i, j)])) return is;
    m = tmp;
    return is;
}

// 3 "What would be a more complete set of members for Matrix?"
//(discussion, requested by the exercise; not all implemented,
//since the exercise only asks for a list + brief argument, plus the += operator which IS implemented above)


// main(): demo/test of every exercise.
int main() {
    cout << "=== [1] construct, ==, [], + ===\n";
    Matrix a(2, 3);
    a(0, 0) = 1; a(0, 1) = 2; a(0, 2) = 3;
    a(1, 0) = 4; a(1, 1) = 5; a(1, 2) = 6;

    Matrix b(2, 3, 1.0);   // every element = 1.0

    Matrix c = a + b;
    cout << "a:\n" << a;
    cout << "b:\n" << b;
    cout << "a+b:\n" << c;
    cout << "a == a? " << boolalpha << (a == a) << "\n";
    cout << "a == b? " << (a == b) << "\n";

    try {
        Matrix bad(3, 3);
        a + bad;   // dimension mismatch -> should throw
    } catch (const exception& e) {
        cout << "expected error caught: " << e.what() << "\n";
    }

    try {
        a(5, 0) = 1;   // out of range -> should throw
    } catch (const exception& e) {
        cout << "expected error caught: " << e.what() << "\n";
    }

    cout << "\n=== [2] << and >> ===\n";
    cout << "streamed a:\n" << a;
    istringstream iss("2 2\n9 8\n7 6\n");
    Matrix d(1, 1);
    iss >> d;
    cout << "read back d:\n" << d;

    cout << "\n=== [3] += (see comment block above main() for the full discussion) ===\n";
    Matrix e(2, 3, 10.0);
    e += a;
    cout << "e += a:\n" << e;

    cout << "\n=== [4] row(i) / column(i) ===\n";
    cout << "a.row(1): ";
    for (double v : a.row(1)) cout << v << ' ';
    cout << "\na.column(2): ";
    for (double v : a.column(2)) cout << v << ' ';
    cout << "\n";

    return 0;
}