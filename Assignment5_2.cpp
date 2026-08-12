#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

using namespace std;

// [1] 
void ex1_pointer_format() {
    int x = 42;
    int* p = &x;
    cout << "ex1: address of x printed as: " << p << "\n";
}

// [2] 
void ex2_type_sizes() {
    // We know the byte counts from the platform's common convention
    // and only use sizeof() afterward to confirm we were right.
    cout << "ex2: expected int=4 bytes, double=8 bytes, bool=1 byte\n";
    cout << "ex2: sizeof(int)="  << sizeof(int)
         << " sizeof(double)="   << sizeof(double)
         << " sizeof(bool)="     << sizeof(bool) << "\n";
}

// [3] 
void ex3_pointer_misuse_examples() {
    cout << "ex3: two classic disasters (described, not executed, since\n"
         << "     they'd crash or corrupt memory):\n"
         << "  1) Dangling pointer: returning the address of a local\n"
         << "     variable/deleting an object then using the pointer\n"
         << "     again, e.g.:\n"
         << "        int* p = new int(5); delete p; *p = 10; // UB, p dangles\n"
         << "  2) Buffer overrun: writing past the end of an allocated\n"
         << "     array through a pointer, e.g.:\n"
         << "        int* a = new int[3]; a[5] = 1; // writes out of bounds\n";
}

// [4] 
int global_var = 0;                 // lives in static storage

void probe_stack(int depth, uintptr_t* addrs) {
    int local = 0;                   // lives on the stack
    addrs[depth] = reinterpret_cast<uintptr_t>(&local);
    if (depth > 0) probe_stack(depth - 1, addrs);
}

void ex4_memory_layout() {
    static int static_var = 0;       // also static storage
    int stack_var = 0;               // stack
    int* heap_var = new int(0);      // free store

    cout << "ex4: static_var @ " << &static_var
         << ", global_var @ " << &global_var
         << ", stack_var @ " << &stack_var
         << ", heap_var @ " << heap_var << "\n";

    // Observe how the stack address changes as recursion goes deeper.
    uintptr_t addrs[5];
    probe_stack(4, addrs);
    cout << "ex4: stack addresses at increasing recursion depth: ";
    for (int i = 4; i >= 0; --i) cout << addrs[i] << ' ';
    cout << "\n";
    cout << "ex4: (addrs[4] is the outermost call). If the address\n"
         << "     DECREASES as depth increases, the stack grows toward\n"
         << "     lower addresses (the typical case on x86/x86-64).\n";

    int* arr = new int[5];
    cout << "ex4: free-store array: ";
    for (int i = 0; i < 5; ++i) cout << &arr[i] << ' ';
    cout << "\n     -> addresses increase with index, i.e. arr[i] is at a\n"
         << "        higher address than arr[i-1] (arrays are laid out\n"
         << "        contiguously in increasing-address order).\n";

    delete heap_var;
    delete[] arr;
}

// [5] 
void ex5_memory_exhaustion_demo() {
    cout << "ex5: DOCUMENTED BEHAVIOR: when new cannot obtain memory it\n"
         << "     throws std::bad_alloc (unless a nothrow overload is used,\n"
         << "     in which case it returns nullptr).\n";
    cout << "ex5: EXPERIMENTAL: allocating 1MB blocks until failure...\n";
    long blocks = 0;
    const size_t block_size = 1024 * 1024;   
    try {
        vector<char*> keep;                  // keep pointers so blocks aren't "lost"
        while (true) {
            keep.push_back(new char[block_size]);
            ++blocks;
            if (blocks % 50 == 0) cout << "  allocated " << blocks << " MB so far...\n";
            if (blocks > 300) { cout << "  (stopping demo early -- real exhaustion would take much longer)\n"; break; }
        }
        for (char* p : keep) delete[] p;
    } catch (const bad_alloc&) {
        cout << "ex5: caught std::bad_alloc after " << blocks << " MB allocated.\n";
    }
    cout << "ex5: exact figure varies by machine/OS; what matters for the\n"
         << "     defense is the mechanism (bad_alloc thrown), not the number.\n";
}

// [6] 
void ex6_read_into_free_store_array(istream& in) {
    const int MAX = 1000;
    char* buf = new char[MAX];
    int n = 0;
    char c;
    while (n < MAX - 1 && in.get(c) && c != '!')
        buf[n++] = c;
    buf[n] = '\0';
    cout << "ex6: read " << n << " chars: \"" << buf << "\"\n";
    delete[] buf;
}

// [7] 
void ex7_read_into_string(istream& in) {
    string s;
    char c;
    while (in.get(c) && c != '!')
        s += c;
    cout << "ex7: read " << s.size() << " chars: \"" << s << "\"\n";
}

// [8] 
void ex8_growth_directions() {
    cout << "ex8: see ex4's output -- recursive stack-address probe shows\n"
         << "     the stack growing toward LOWER addresses on this platform;\n"
         << "     successive `new`s below show the free store growing toward\n"
         << "     HIGHER addresses (the allocator typically hands out memory\n"
         << "     from a heap that extends upward).\n";
    int* a = new int;
    int* b = new int;
    int* c = new int;
    cout << "ex8: new addresses: " << a << " " << b << " " << c << "\n";
    delete a; delete b; delete c;
}

// [9] 
void ex9_overflow_discussion() {
    cout << "ex9: ex6 as written CANNOT overflow, because the loop condition\n"
         << "     `n < MAX - 1` stops writing once the buffer is full -- extra\n"
         << "     input characters are simply left unread rather than causing\n"
         << "     an out-of-bounds write. If that bound check were removed,\n"
         << "     typing more than MAX-1 characters before '!' would write\n"
         << "     past the end of the array: undefined behavior (silent\n"
         << "     corruption of adjacent free-store memory, possibly a crash\n"
         << "     much later and far away from the actual bug).\n";
}

// [10]/[11]/[12]/[13]/[14] "List of gods" example from Stroustrup
struct God {
    string name, mythology, description, weapon;
};

ostream& operator<<(ostream& os, const God& g) {
    return os << g.name << " (" << g.mythology << "): " << g.weapon;
}

struct Link {
    God value;
    Link* prev;
    Link* succ;
    Link(const God& v, Link* p = nullptr, Link* s = nullptr)
        : value(v), prev(p), succ(s) {}
};

// insert n after p in the list; returns a pointer to the new element
Link* insert(Link* p, Link* n) {   // "insert n before p"
    if (p == nullptr) return n;
    n->succ = p;
    if (p->prev) p->prev->succ = n;
    n->prev = p->prev;
    p->prev = n;
    return n;
}

Link* add(Link* p, Link* n) {      // "insert n after p"
    if (p == nullptr) return n;
    n->prev = p;
    if (p->succ) p->succ->prev = n;
    n->succ = p->succ;
    p->succ = n;
    return n;
}

Link* erase(Link* p) {             // remove *p, return the following element
    if (p == nullptr) return nullptr;
    if (p->succ) p->succ->prev = p->prev;
    if (p->prev) p->prev->succ = p->succ;
    Link* next = p->succ;
    delete p;                     
    return next;
}

Link* find(Link* start, const string& name) {
    for (Link* p = start; p; p = p->succ)
        if (p->value.name == name) return p;
    return nullptr;
}
const Link* find(const Link* start, const string& name) {
    for (const Link* p = start; p; p = p->succ)
        if (p->value.name == name) return p;
    return nullptr;
}

// [10] print_all() list gods with attributes, one per line.
void print_all(const Link* start) {
    for (const Link* p = start; p; p = p->succ)
        cout << "  " << p->value.name << " | " << p->value.mythology
             << " | " << p->value.description << " | " << p->value.weapon << "\n";
}

// [12] 
Link* add_ordered(Link* head, Link* n) {
    if (head == nullptr || n->value.name < head->value.name) {
        n->succ = head;
        if (head) head->prev = n;
        n->prev = nullptr;
        return n;                              // n becomes the new head
    }
    Link* p = head;
    while (p->succ && p->succ->value.name < n->value.name)
        p = p->succ;
    add(p, n);
    return head;
}

// [13] 
void delete_list(Link* head) {
    while (head) head = erase(head);
}

void ex10_12_13_gods_demo() {
    cout << "ex10/12/13: build a Greek list, ordered by name, then print & free it\n";
    Link* greek = nullptr;
    God zeus{"Zeus", "Greek", "king of the gods", "lightning"};
    God hera{"Hera", "Greek", "queen of the gods", "royal sceptre"};
    God ares{"Ares", "Greek", "god of war", "spear"};
    greek = add_ordered(greek, new Link(zeus));
    greek = add_ordered(greek, new Link(hera));
    greek = add_ordered(greek, new Link(ares));   // exercises add_ordered's "new head" path
    print_all(greek);
    delete_list(greek);                            // no leak: every Link is delete-d exactly once
}

// [14] 
struct SLink {
    God value;
    SLink* succ;
    SLink(const God& v, SLink* s = nullptr) : value(v), succ(s) {}
};

SLink* sl_add_ordered(SLink* head, SLink* n) {
    if (head == nullptr || n->value.name < head->value.name) {
        n->succ = head;
        return n;
    }
    SLink* p = head;
    while (p->succ && p->succ->value.name < n->value.name)
        p = p->succ;
    n->succ = p->succ;
    p->succ = n;
    return head;
}

void sl_print_all(const SLink* start) {
    for (const SLink* p = start; p; p = p->succ)
        cout << "  " << p->value.name << " | " << p->value.mythology
             << " | " << p->value.weapon << "\n";
}

void sl_delete_list(SLink* head) {
    while (head) { SLink* next = head->succ; delete head; head = next; }
}

void ex14_singly_linked_demo() {
    cout << "ex14: same gods, singly-linked list version\n";
    SLink* norse = nullptr;
    God odin{"Odin", "Norse", "the Allfather", "Gungnir"};
    God thor{"Thor", "Norse", "god of thunder", "Mjolnir"};
    norse = sl_add_ordered(norse, new SLink(odin));
    norse = sl_add_ordered(norse, new SLink(thor));
    sl_print_all(norse);
    sl_delete_list(norse);
}

// main(): menu-driven demo of every exercise.
int main() {
    cout << "=== [1] pointer output format ===\n"; ex1_pointer_format();
    cout << "\n=== [2] sizes of int/double/bool ===\n"; ex2_type_sizes();
    cout << "\n=== [3] two pointer-misuse examples ===\n"; ex3_pointer_misuse_examples();
    cout << "\n=== [4] memory layout & stack growth direction ===\n"; ex4_memory_layout();
    cout << "\n=== [5] memory exhaustion ===\n"; ex5_memory_exhaustion_demo();

    cout << "\n=== [6] read chars into free-store array until '!' ===\n";
    { istringstream demo_in("hello world!"); ex6_read_into_free_store_array(demo_in); }

    cout << "\n=== [7] same, but into a std::string ===\n";
    { istringstream demo_in("hello world!"); ex7_read_into_string(demo_in); }

    cout << "\n=== [8] stack/free-store growth direction ===\n"; ex8_growth_directions();
    cout << "\n=== [9] could ex6 overflow? ===\n"; ex9_overflow_discussion();
    cout << "\n=== [10]/[12]/[13] list of gods: print_all, add_ordered, no leaks ===\n";
    ex10_12_13_gods_demo();
    cout << "\n=== [11] why two find() overloads -- see comment above find() ===\n";
    cout << "\n=== [14] singly-linked reimplementation ===\n"; ex14_singly_linked_demo();

    return 0;
}