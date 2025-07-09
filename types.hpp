#ifndef TYPES_HPP
#define TYPES_HPP

#include <ostream>
using namespace std;

struct Complex {
    long double real;
    long double imag;

    Complex() {
        real = 0.0;
        imag = 0.0;
    }

    Complex(long double r, long double i) {
        real = r;
        imag = i;
    }

    Complex operator+(const Complex& other) const {
        return Complex (real + other.real, imag + other.imag);
    }

    Complex square() const {
        return Complex(real * real - imag * imag, real * imag * 2);
    }

    long double magnitude() const {
        return real * real + imag * imag;
    }
};

inline ostream& operator<<(ostream& os, const Complex& c) {
    os << c.real << " + " << c.imag << "i";
    return os;
}

class Mandelbrot {
    public:
    Mandelbrot(Complex o) {
        origin = o;
        nth_iteration = 0;
        nth_number = Complex();
    }

    bool InsideMandelbrot() {
        return (4.0 > nth_number.magnitude());
    }

    private:
    Complex origin;
    unsigned int nth_iteration;
    Complex nth_number;
};

#endif