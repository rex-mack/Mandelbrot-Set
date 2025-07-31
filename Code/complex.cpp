#include <ostream>
#include "complex.hpp"

Complex::Complex() {
    real = 0.0;
    imaginary = 0.0;
}
Complex::Complex(double r, double i) {
    real = r;
    imaginary = i;
}
double Complex::getReal() const {
    return real;
}
double Complex::getImag() const {
    return imaginary;
}
Complex Complex::operator+(const Complex& other) const {
    return Complex(other.real + real, other.imaginary + imaginary);
}
Complex Complex::operator-(const Complex& other) const {
    return Complex(other.real - real, other.imaginary - imaginary);
}
Complex Complex::operator*(const Complex& other) const {
    return Complex((real * other.real) - (imaginary * other.imaginary),
                   (real * other.imaginary) + (imaginary * other.real));
}
bool Complex::operator==(const Complex& other) const {
    return (real == other.real) && (imaginary == other.imaginary);
}
double Complex::magnitudeSquared() const {
    return (real*real) + (imaginary*imaginary);
}
std::ostream& operator<<(std::ostream& out, const Complex& c) {
    out << '(' << c.getReal() << " + " << c.getImag() << 'i' << ')';
    return out;
}