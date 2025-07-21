#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include "config.h"

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
    double real;
    double imaginary;

    public:
    Complex();
    Complex(double r, double i); // set the number (real part, imaginary part)

    double getReal() const; // returns real part
    double getImag() const; // returns imaginary part

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex square() const; // retures the number^2
    double magnitudeSquared() const; // returns magnitude squared
};

std::ostream& operator<<(std::ostream& out, const Complex& c) ;

#endif