#include "config.h"
using namespace std;

/*
    Complex number in form a + bi
        where a is the real part and i is the imaginary part

    added functions for:
        setting number
        getting real part or imaginary part

        operator+
        operator<<
        square of the number
        magnitude squared of the number
*/
class Complex {
    private:
    long double real;
    long double imag;

    public:
    Complex();
    Complex(long double r, long double i); // set the number (real part, imaginary part)

    long double getReal() const; // returns real part
    long double getImag() const; // returns imaginary part

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex square() const; // retures the number^2
    long double magnitudeSquared() const; // returns magnitude squared
};
