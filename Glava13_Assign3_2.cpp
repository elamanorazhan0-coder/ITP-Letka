// Exercises 13.1 - 13.7
#include <iostream>
#include <stdexcept>
using namespace std;

// 13.1  Basic try/throw/catch

namespace Ex13_1 {

void demo() {
    cout << "\n--- 13.1 Basic try/throw/catch ---\n";
    int a = 10, b = 0;
    try {
        if (b == 0)
            throw runtime_error("Division by zero!");   // the "possible exception"
        cout << a / b << "\n";
    }
    catch (const runtime_error &e) {
        cout << "Caught: " << e.what() << "\n";
    }
}

} // namespace Ex13_1

// 13.2  Multiple catch statements for one try block
namespace Ex13_2 {

void test(int choice) {
    try {
        if (choice == 1) throw 100;                 // int
        if (choice == 2) throw 3.14;                // double
        if (choice == 3) throw string("text error"); // string
    }
    catch (int e) {
        cout << "Caught an int exception: " << e << "\n";
    }
    catch (double e) {
        cout << "Caught a double exception: " << e << "\n";
    }
    catch (const string &e) {
        cout << "Caught a string exception: " << e << "\n";
    }
}

void demo() {
    cout << "\n--- 13.2 Multiple catch blocks ---\n";
    // running all three so we can see each catch block fire in turn
    test(1);
    test(2);
    test(3);
}

} // namespace Ex13_2

// 13.3  catch(...) - catches literally anything
namespace Ex13_3 {

void demo() {
    cout << "\n--- 13.3 catch(...) handler ---\n";
    try {
        throw 'x';   // could be any type at all, doesn't matter here
    }
    catch (...) {
        // catch(...) doesn't give us access to the actual exception
        // value - it's a catch-all safety net, not meant for details
        cout << "Caught something with catch(...) - type unknown/unimportant.\n";
    }
}

} // namespace Ex13_3

// 13.4  Some exception types cannot be thrown as-is
//       (classic example: you can't throw an incomplete type,
//       or an array/function type - only complete object types).
//       Demonstrating with array vs. pointer.
namespace Ex13_4 {

void demo() {
    cout << "\n--- 13.4 Types that are not allowed to be thrown ---\n";
    // Uncommenting the line below causes a COMPILE error in C++:
    //     int arr[5];
    //     throw arr;          // error: cannot throw array type 'int[5]'
    // Arrays decay in most contexts, but exception-throwing does not
    // allow raw array or function types - only object types, pointers,
    // references, or class instances can be thrown.

    // What IS allowed: throwing a pointer to the array instead.
    int arr[5] = {1, 2, 3, 4, 5};
    try {
        throw arr;   // arr decays to int* here, so this actually throws an int*
    }
    catch (int *p) {
        cout << "Caught a pointer, first element = " << p[0] << "\n";
    }
    cout << "(Raw array/function types cannot be thrown directly - only\n"
         << " object types, pointers and references can be exceptions.)\n";
}

} // namespace Ex13_4

// 13.5  Rethrowing an exception
namespace Ex13_5 {

void innerFunction() {
    throw runtime_error("Error from innerFunction");
}

void middleFunction() {
    try {
        innerFunction();
    }
    catch (const runtime_error &e) {
        cout << "middleFunction caught it, logging: " << e.what() << "\n";
        throw;   // rethrow the SAME exception up to the caller
    }
}

void demo() {
    cout << "\n--- 13.5 Rethrowing an exception ---\n";
    try {
        middleFunction();
    }
    catch (const runtime_error &e) {
        cout << "main-level catch got it too: " << e.what() << "\n";
    }
}

} // namespace Ex13_5

// 13.6  Read two doubles, divide them, handle bad input AND divide-by-zero
namespace Ex13_6 {

// (a) read two doubles from the keyboard
bool readTwoNumbers(double &a, double &b) {
    cout << "Enter two numbers: ";
    cin >> a >> b;
    return (bool)cin;   // false if extraction failed (wrong data type typed)
}

// (b) compute the division
double divide(double a, double b) {
    if (b == 0)
        throw runtime_error("Divide-by-zero!");   // (d)
    return a / b;
}

void demo() {
    cout << "\n--- 13.6 Safe division with input validation ---\n";
    double a, b;

    try {
        if (!readTwoNumbers(a, b)) {
            cin.clear();
            cin.ignore(10000, '\n');
            throw invalid_argument("Wrong type of data entered!");   // (c)
        }
        cout << "Result: " << divide(a, b) << "\n";   // (d) throws inside divide()
    }
    catch (const invalid_argument &e) {   // (e) handles bad input
        cout << "Input error: " << e.what() << "\n";
    }
    catch (const runtime_error &e) {      // (e) handles divide-by-zero
        cout << "Math error: " << e.what() << "\n";
    }
}

} // namespace Ex13_6

// 13.7  Exception thrown deep inside nested function calls
namespace Ex13_7 {

void level3() {
    cout << "Inside level3()...\n";
    throw runtime_error("Something broke deep in level3()");
}

void level2() {
    cout << "Inside level2(), calling level3()...\n";
    level3();   // no try/catch here - exception just keeps propagating up
}

void level1() {
    cout << "Inside level1(), calling level2()...\n";
    level2();
}

void demo() {
    cout << "\n--- 13.7 Exception from a deeply nested function ---\n";
    try {
        level1();   // the only place we actually catch anything
    }
    catch (const runtime_error &e) {
        cout << "Caught in main-level try block: " << e.what() << "\n";
    }
}

} // namespace Ex13_7

int main() {
    Ex13_1::demo();
    Ex13_2::demo();
    Ex13_3::demo();
    Ex13_4::demo();
    Ex13_5::demo();
    Ex13_6::demo();   // needs keyboard input when run for real
    Ex13_7::demo();
    return 0;
}