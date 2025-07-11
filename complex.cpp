#include "complex.hpp"
#include "config.h"
#include <iostream>

using namespace std;

Complex::Complex() {
    real = 0.0;
    imag = 0.0;
}
Complex::Complex(long double r, long double i) {
    real = r;
    imag = i;
}
long double Complex::getReal() const {
    return real;
}
long double Complex::getImag() const {
    return imag;
}
Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}
Complex Complex::operator-(const Complex& other) const {
    return Complex(other.real - real, other.imag - imag);
}
Complex Complex::square() const {
    return Complex((real*real) - (imag*imag), 2*real*imag);
}
long double Complex::magnitudeSquared() const {
    return (real*real) + (imag*imag);
}