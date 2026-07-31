// Exercises 9.1, 9.2, 9.3
#include <iostream>
using namespace std;

// 9.1 & 9.2  shape (base) -> triangle, rectangle, circle
namespace Ex9_1_2 {

class shape {
protected:
    double x, y;
public:
    void get_data(double a, double b = 0.0) {
        x = a;
        y = b;
    }
    virtual void display_area() = 0;   // pure virtual: shape itself has no area
    virtual ~shape() {}
};

class triangle : public shape {
public:
    void display_area() override {
        double area = 0.5 * x * y;
        cout << "Triangle area (base=" << x << ", height=" << y << "): " << area << "\n";
    }
};

class rectangle : public shape {
public:
    void display_area() override {
        double area = x * y;
        cout << "Rectangle area (sides " << x << " x " << y << "): " << area << "\n";
    }
};

// 9.2: circle needs only radius; y defaults to 0 in get_data()
class circle : public shape {
public:
    void display_area() override {
        double area = 3.14159 * x * x;   // x holds the radius
        cout << "Circle area (radius=" << x << "): " << area << "\n";
    }
};

void demo() {
    cout << "\n--- 9.1 / 9.2 shape hierarchy with virtual display_area() ---\n";
    shape *shapes[3];
    shapes[0] = new triangle;
    shapes[1] = new rectangle;
    shapes[2] = new circle;

    shapes[0]->get_data(6, 4);      // base=6, height=4
    shapes[1]->get_data(5, 8);      // sides 5 x 8
    shapes[2]->get_data(7);         // radius=7, second arg defaults to 0

    for (int i = 0; i < 3; i++) {
        shapes[i]->display_area();  // virtual call -> correct override chosen at runtime
        delete shapes[i];
    }
}

} // namespace Ex9_1_2

// 9.3  Two variations to observe/compile & comment on:
//
//  (a) Remove display_area() from one derived class
//      (say, rectangle) while shape::display_area() is a
//      *non-virtual, non-pure* function with a body.
//      -> Since it is NOT virtual, calling display_area()
//         through a base-class pointer always calls
//         shape::display_area()
//
//  (b) Additionally declare display_area() as virtual in the
//      base class.
//      -> Now the call is resolved at runtime (dynamic
//         binding). Since 'rectangle' still doesn't override
//         it
namespace Ex9_3 {

// (a) base class display_area() is NOT virtual, has a body,
//     rectangle does not redefine it.
class shape_a {
protected:
    double x, y;
public:
    void get_data(double a, double b = 0.0) { x = a; y = b; }
    void display_area() {   // NOT virtual
        cout << "shape_a::display_area() called - no area computed (x=" << x << ", y=" << y << ")\n";
    }
};

class triangle_a : public shape_a {
public:
    void display_area() {
        cout << "Triangle area: " << 0.5 * x * y << "\n";
    }
};

// rectangle_a intentionally does NOT define display_area()
class rectangle_a : public shape_a {
    // (empty on purpose - relies on shape_a::display_area())
};

void demo_a() {
    cout << "\n--- 9.3(a): base display_area() NOT virtual, rectangle has no override ---\n";
    shape_a *s1 = new triangle_a;
    shape_a *s2 = new rectangle_a;
    s1->get_data(6, 4);
    s2->get_data(5, 8);

    s1->display_area();   // resolved at compile time -> shape_a* type -> shape_a::display_area() ALWAYS
    s2->display_area();   // same here: static binding means base version runs even though object is rectangle_a
    cout << "Comment: with NO virtual keyword, the base class pointer type alone\n"
         << "decides which display_area() runs (compile-time/static binding).\n"
         << "Even s1 (really a triangle_a) calls shape_a::display_area() through\n"
         << "a shape_a* pointer -- polymorphism does NOT happen here.\n";
    delete s1;
    delete s2;
}

// (b) base class display_area() IS virtual, rectangle still
//     does not redefine it.
class shape_b {
protected:
    double x, y;
public:
    void get_data(double a, double b = 0.0) { x = a; y = b; }
    virtual void display_area() {   // now virtual, with a body (not pure)
        cout << "shape_b::display_area() called - no specific shape (x=" << x << ", y=" << y << ")\n";
    }
    virtual ~shape_b() {}
};

class triangle_b : public shape_b {
public:
    void display_area() override {
        cout << "Triangle area: " << 0.5 * x * y << "\n";
    }
};

// rectangle_b intentionally does NOT override display_area()
class rectangle_b : public shape_b {
    // (empty on purpose)
};

void demo_b() {
    cout << "\n--- 9.3(b): base display_area() IS virtual, rectangle has no override ---\n";
    shape_b *s1 = new triangle_b;
    shape_b *s2 = new rectangle_b;
    s1->get_data(6, 4);
    s2->get_data(5, 8);

    s1->display_area();   // dynamic binding -> triangle_b's own version runs
    s2->display_area();   // dynamic binding, but rectangle_b has no override
                           // -> falls back to inherited shape_b::display_area()
    cout << "Comment: with 'virtual', the call is resolved at RUNTIME based on\n"
         << "the actual object type. triangle_b correctly uses its own override.\n"
         << "rectangle_b has none, so it inherits and runs shape_b::display_area()\n"
         << "(the nearest ancestor's version) instead of causing an error.\n";
    delete s1;
    delete s2;
}

} // namespace Ex9_3

int main() {
    Ex9_1_2::demo();
    Ex9_3::demo_a();
    Ex9_3::demo_b();
    return 0;
}