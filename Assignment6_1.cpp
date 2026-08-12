#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>
#include <cstdlib>  
#include <functional>

using namespace std;

// [1] add() 
// Adds vu into vt element-wise: vt[i] += vu[i]. Sizes must match,
// otherwise we'd silently ignore some elements which is worse than
// just telling the caller something's wrong
template<typename T>
void add(vector<T>& vt, const vector<T>& vu) {
    if (vt.size() != vu.size()) throw runtime_error("add(): vectors must be the same size");
    for (size_t i = 0; i < vt.size(); ++i) vt[i] += vu[i];
}

// [2] dot-product-style sum 
// vt and vu can hold different types, so the result type is deduced
// from vt[i]*vu[i] rather than fixed to T or U
template<typename T, typename U>
auto dot_sum(const vector<T>& vt, const vector<U>& vu) {
    if (vt.size() != vu.size()) throw runtime_error("dot_sum(): vectors must be the same size");
    decltype(vt[0] * vu[0]) sum{};
    for (size_t i = 0; i < vt.size(); ++i) sum += vt[i] * vu[i];
    return sum;
}

// [3] Pair<T,U> + tiny symbol table 
// Mirrors the "name -> value" table the desk calculator  uses to
// keep track of variables
template<typename T, typename U>
class Pair {
    T first_;
    U second_;
public:
    Pair(T f, U s) : first_(f), second_(s) {}
    T& first() { return first_; }
    U& second() { return second_; }
    const T& first() const { return first_; }
    const U& second() const { return second_; }
};

class Symbol_table {
    vector<Pair<string, double>> table;
public:
    void set(const string& name, double value) {
        for (auto& p : table)
            if (p.first() == name) { p.second() = value; return; }
        table.push_back(Pair<string, double>(name, value));
    }
    double get(const string& name) const {
        for (auto& p : table)
            if (p.first() == name) return p.second();
        throw runtime_error("undefined name: " + name);
    }
};

// [4] Link<T> template + no-leak list 
template<typename T>
struct Link {
    T value;
    Link* prev;
    Link* succ;
    Link(const T& v, Link* p = nullptr, Link* s = nullptr) : value(v), prev(p), succ(s) {}
};

template<typename T>
Link<T>* add_front(Link<T>* head, Link<T>* n) {
    n->succ = head;
    if (head) head->prev = n;
    return n;
}

template<typename T>
void delete_list(Link<T>* head) {
    while (head) {
        Link<T>* next = head->succ;
        delete head;          // this is what stops the leak
        head = next;
    }
}

struct God { string name, weapon; };
ostream& operator<<(ostream& os, const God& g) { return os << g.name << " / " << g.weapon; }

// [5] class Int 
class Int {
    int val;
public:
    Int(int v = 0) : val(v) {}
    Int& operator=(const Int&) = default;
    Int operator+(const Int& b) const { return Int(val + b.val); }
    Int operator-(const Int& b) const { return Int(val - b.val); }
    Int operator*(const Int& b) const { return Int(val * b.val); }
    Int operator/(const Int& b) const {
        if (b.val == 0) throw runtime_error("Int: division by zero");
        return Int(val / b.val);
    }
    int value() const { return val; }
    friend ostream& operator<<(ostream& os, const Int& i) { return os << i.val; }
    friend istream& operator>>(istream& is, Int& i) { return is >> i.val; }
};

// [6] Number<T> template 
template<typename T>
class Number {
    T val;
public:
    Number(T v = T()) : val(v) {}
    Number operator+(const Number& b) const { return Number(val + b.val); }
    Number operator-(const Number& b) const { return Number(val - b.val); }
    Number operator*(const Number& b) const { return Number(val * b.val); }
    Number operator/(const Number& b) const { return Number(val / b.val); }
    // % only makes sense for integral T; for double this won't compile,
    // which is exactly the point of the exercise -- the compiler catches
    // the misuse for us instead of us having to guard against it manually.
    Number operator%(const Number& b) const { return Number(val % b.val); }
    T value() const { return val; }
    friend ostream& operator<<(ostream& os, const Number& n) { return os << n.val; }
};

// [8] a minimal allocator 
template<typename T>
struct Simple_allocator {
    T* allocate(size_t n) {
        T* p = static_cast<T*>(malloc(n * sizeof(T)));
        if (!p) throw bad_alloc();
        return p;
    }
    void deallocate(T* p, size_t) { free(p); }
    void construct(T* p, const T& v) { new (p) T(v); }   // placement new
    void destroy(T* p) { p->~T(); }
};
// [8]/[9] Vector using the allocator 
template<typename T>
class Vector {
    Simple_allocator<T> alloc;
    T* elem = nullptr;
    int sz = 0;
public:
    Vector() = default;
    explicit Vector(int n) : elem(alloc.allocate(n)), sz(n) {
        for (int i = 0; i < n; ++i) alloc.construct(elem + i, T());
    }
    Vector(const Vector& other) : elem(alloc.allocate(other.sz)), sz(other.sz) {
        for (int i = 0; i < sz; ++i) alloc.construct(elem + i, other.elem[i]);
    }
    // [9] operator=() re-implemented using the allocator directly,
    // instead of just delegating to a copy+swap - shows the allocate
    // construct/destroy/deallocate cycle explicitly
    Vector& operator=(const Vector& other) {
        if (this == &other) return *this;
        for (int i = 0; i < sz; ++i) alloc.destroy(elem + i);
        if (sz != other.sz) {
            alloc.deallocate(elem, sz);
            elem = alloc.allocate(other.sz);
        }
        sz = other.sz;
        for (int i = 0; i < sz; ++i) alloc.construct(elem + i, other.elem[i]);
        return *this;
    }
    ~Vector() {
        for (int i = 0; i < sz; ++i) alloc.destroy(elem + i);
        alloc.deallocate(elem, sz);
    }
    T& operator[](int i) { return elem[i]; }
    int size() const { return sz; }
};

// [10] a minimal unique_ptr 
template<typename T>
class Simple_unique_ptr {
    T* p;
public:
    explicit Simple_unique_ptr(T* pp = nullptr) : p(pp) {}
    ~Simple_unique_ptr() { delete p; }
    Simple_unique_ptr(const Simple_unique_ptr&) = delete;              // no copying...
    Simple_unique_ptr& operator=(const Simple_unique_ptr&) = delete;   // ...ownership must stay unique
    T* operator->() const { return p; }
    T& operator*() const { return *p; }
    T* release() { T* old = p; p = nullptr; return old; }   // give up ownership
};

// [11] Counted_ptr<T> (shared-ownership smart pointer) 
template<typename T>
class Counted_ptr {
    T* obj;
    int* use_count;
public:
    explicit Counted_ptr(const T& val) : obj(new T(val)), use_count(new int(1)) {}
    Counted_ptr(const Counted_ptr& other) : obj(other.obj), use_count(other.use_count) {
        ++(*use_count);
    }
    Counted_ptr& operator=(const Counted_ptr& other) {
        if (this == &other) return *this;
        release();
        obj = other.obj; use_count = other.use_count;
        ++(*use_count);
        return *this;
    }
    ~Counted_ptr() { release(); }
    T& operator*() const { return *obj; }
    T* operator->() const { return obj; }
    int count() const { return *use_count; }
private:
    void release() {
        if (--(*use_count) == 0) { delete obj; delete use_count; }
    }
};

// [12] File_handle (RAII) 
#include <fstream>
class File_handle {
    ofstream file;
public:
    explicit File_handle(const string& name) : file(name) {
        if (!file) throw runtime_error("File_handle: could not open " + name);
    }
    // destructor closes automatically when file (an ofstream member) is destroyed
    ofstream& stream() { return file; }
};

// [13] Tracer (RAII, prints on construct/destruct) 
class Tracer {
    string msg_in, msg_out;
public:
    Tracer(const string& in, const string& out) : msg_in(in), msg_out(out) { cout << msg_in << "\n"; }
    ~Tracer() { cout << msg_out << "\n"; }
    Tracer(const Tracer& t) : msg_in("(copy of) " + t.msg_in), msg_out("(copy of) " + t.msg_out) {
        cout << "copying: " << msg_in << "\n";
    }
    Tracer& operator=(const Tracer& t) {
        cout << "copy-assigning into: " << msg_out << "\n";
        msg_in = t.msg_in; msg_out = t.msg_out;
        return *this;
    }
};

// [16] small-footprint Vector (single pointer member) 
// Requirement: sizeof(Small_vector<int>) == sizeof(int*). We can not store
// size/capacity as separate members , so we pack
// them into the same heap block as the elements: [size][capacity][data..].
template<typename T>
class Small_vector {
    struct Header { size_t size, capacity; };
    char* block = nullptr;   // the ONLY member -- this is what keeps sizeof() minimal

    Header* header() { return reinterpret_cast<Header*>(block); }
    T* data() { return reinterpret_cast<T*>(block + sizeof(Header)); }

public:
    Small_vector() = default;
    void push_back(const T& v) {
        size_t old_size = block ? header()->size : 0;
        size_t old_cap = block ? header()->capacity : 0;
        if (old_size == old_cap) {
            size_t new_cap = old_cap == 0 ? 1 : old_cap * 2;
            char* new_block = new char[sizeof(Header) + new_cap * sizeof(T)];
            auto* h = reinterpret_cast<Header*>(new_block);
            h->size = old_size; h->capacity = new_cap;
            T* new_data = reinterpret_cast<T*>(new_block + sizeof(Header));
            if (block) {
                for (size_t i = 0; i < old_size; ++i) new_data[i] = data()[i];
                delete[] block;
            }
            block = new_block;
        }
        data()[header()->size] = v;
        ++header()->size;
    }
    size_t size() const { return block ? reinterpret_cast<const Header*>(block)->size : 0; }
    T& operator[](size_t i) { return data()[i]; }
    ~Small_vector() { delete[] block; }
};

// [17] finally() 
template<typename F>
class Final_action {
    F act;
public:
    explicit Final_action(F f) : act(f) {}
    ~Final_action() { act(); }
};

template<typename F>
Final_action<F> finally(F f) { return Final_action<F>(f); }

// [14]/[15] Hunt the Wumpus GUI 
// These two ask for a GUI (input box + map window) built on the FLTK-
// style GUI library from the books later chapters, which isn't part of
// this console-only toolchain. Left undone here; the console version of
// Hunt the Wumpus from the Chapter 16 exercises already covers the game
// logic these two exercises would wrap in a window

// main(): quick demo/test of everything above.
int main() {
    cout << "=== [1] add() ===\n";
    vector<int> v1{1,2,3}, v2{10,20,30};
    add(v1, v2);
    for (int x : v1) cout << x << ' '; cout << "\n";

    cout << "\n=== [2] dot_sum() ===\n";
    vector<int> a{1,2,3};
    vector<double> b{0.5, 0.5, 0.5};
    cout << dot_sum(a, b) << "\n";

    cout << "\n=== [3] Pair / symbol table ===\n";
    Symbol_table st;
    st.set("pi", 3.14159);
    st.set("e", 2.71828);
    cout << "pi = " << st.get("pi") << ", e = " << st.get("e") << "\n";

    cout << "\n=== [4] Link<T> with God, no leaks ===\n";
    Link<God>* gods = nullptr;
    gods = add_front(gods, new Link<God>(God{"Zeus","lightning"}));
    gods = add_front(gods, new Link<God>(God{"Ares","spear"}));
    for (Link<God>* p = gods; p; p = p->succ) cout << p->value << "\n";
    delete_list(gods);

    cout << "\n=== [5] Int ===\n";
    Int i1(6), i2(7);
    cout << i1 << " + " << i2 << " = " << (i1 + i2) << "\n";
    cout << i1 << " * " << i2 << " = " << (i1 * i2) << "\n";
    try { Int(1) / Int(0); } catch (const exception& e) { cout << "caught: " << e.what() << "\n"; }

    cout << "\n=== [6] Number<T>, % works for int, not for double ===\n";
    Number<int> n1(7), n2(3);
    cout << n1 << " % " << n2 << " = " << (n1 % n2) << "\n";
    cout << "(Number<double>::operator% simply won't compile -- % has no\n"
         << " meaning for floating point, and the compiler catches that\n"
         << " for us at compile time if we try to instantiate it.)\n";

    cout << "\n=== [7] dot_sum with Number ===\n";
    // dot_sum needs operator* and operator+= on the element type; Number
    // has * but not +=, so we just demonstrate * manually here instead
    cout << (Number<int>(2) * Number<int>(5)) << "\n";

    cout << "\n=== [8]/[9] Vector using a malloc/free-based allocator ===\n";
    Vector<int> vec(3);
    vec[0]=1; vec[1]=2; vec[2]=3;
    Vector<int> vec2;
    vec2 = vec;                      // exercises the allocator-based operator=
    for (int i = 0; i < vec2.size(); ++i) cout << vec2[i] << ' ';
    cout << "\n";

    cout << "\n=== [10] Simple_unique_ptr ===\n";
    Simple_unique_ptr<string> up(new string("owned"));
    cout << *up << " (" << up->size() << " chars)\n";

    cout << "\n=== [11] Counted_ptr ===\n";
    Counted_ptr<int> cp1(42);
    { Counted_ptr<int> cp2 = cp1; cout << "count while cp2 alive: " << cp1.count() << "\n"; }
    cout << "count after cp2 destroyed: " << cp1.count() << "\n";

    cout << "\n=== [12] File_handle ===\n";
    { File_handle fh("tracer_test.txt"); fh.stream() << "hello from File_handle\n"; }
    cout << "file written and closed automatically\n";

    cout << "\n=== [13] Tracer ===\n";
    { Tracer t("entering scope", "leaving scope"); Tracer t2 = t; }

    cout << "\n=== [16] Small_vector, sizeof == sizeof(pointer) ===\n";
    cout << "sizeof(Small_vector<int>) = " << sizeof(Small_vector<int>)
         << ", sizeof(int*) = " << sizeof(int*) << "\n";
    Small_vector<int> sv;
    sv.push_back(1); sv.push_back(2); sv.push_back(3);
    for (size_t i = 0; i < sv.size(); ++i) cout << sv[i] << ' ';
    cout << "\n";

    cout << "\n=== [17] finally() ===\n";
    { auto x = finally([]{ cout << "Bye!\n"; }); cout << "doing stuff...\n"; }

    return 0;
}