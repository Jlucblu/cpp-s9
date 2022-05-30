#pragma once
#include <iostream>
#include <string>

using namespace std;

struct Circle {
    double x;
    double y;
    double r;
};

struct Dumbbell {
    Circle circle1;
    Circle circle2;
    string text;
};

struct CircleHasher {
    size_t operator() (const Circle& circle) const {
        size_t h_x = d_hasher_(circle.x);
        size_t h_y = d_hasher_(circle.y);
        size_t h_r = d_hasher_(circle.r);

        return h_x + h_y * 37 + h_r * (37 ^ 2);
    }

private:
    hash<double> d_hasher_;
};

struct DumbbellHash {
    size_t operator() (const Dumbbell& dumbbell) const {
        size_t s = s_hasher_(dumbbell.text);
        size_t c1 = c_hasher_(dumbbell.circle1) * (37 ^ 2);
        size_t c2 = c_hasher_(dumbbell.circle2) * (37 ^ 3);
        return s + c1 + c2;
    }

private:
    hash<string> s_hasher_;
    CircleHasher c_hasher_;
};

int main() {
    DumbbellHash hash;
    Dumbbell dumbbell{ {10, 11.5, 2.3}, {3.14, 15, -8}, "abc"s };
    cout << "Dumbbell hash "s << hash(dumbbell);
}