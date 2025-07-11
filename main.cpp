#include <iostream>
#include <fstream>
#include "config.h"
#include "complex.hpp"

using namespace std;

unsigned int mandelbrotEscape(Complex coords, unsigned long iterations) {
    Complex nthIteration = coords;
    unsigned long i = 0;
    while ((i < iterations) && (nthIteration.magnitudeSquared() <= 4)) {
        nthIteration = nthIteration.square() + coords;
        i++;
    }
    return i;
}

bool checkArgs(int argc, char* argv[]) {
    if (argc != 1 && argc != 5) {
        cerr << "Usage:" << endl;
        cerr << "   ./mandelbrot" << endl;
        cerr << "   ./mandelbrot [Max Iterations] [Zoom] [Center Coordinate X] [Center Coordinate I]" << endl;
        return 1;
    }
    if (argc == 1) {
        return 0;
    }

    try {
        stoul(argv[1]);
    } catch (const exception& e) {
        cerr << "Invalid value for Max Iterations: " << argv[1] << endl;
        return 1;
    }
    try {
        stold(argv[2]);
    } catch (const exception& e) {
        cerr << "Invalid value for Zoom: " << argv[2] << endl;
        return 1;
    }
    try {
        stold(argv[3]);
    } catch (const exception& e) {
        cerr << "Invalid value for Center Coordinate X: " << argv[3] << endl;
        return 1;
    }
    try {
        stold(argv[4]);
    } catch (const exception& e) {
        cerr << "Invalid value for Center Coordinate I: " << argv[4] << endl;
        return 1;
    }

    if(stoi(argv[1]) < 0 || stoi(argv[1]) > MAX_ITERATIONS) {
        cerr << "[Max Iterations] must be > 0 or < " << MAX_ITERATIONS << endl;
        return 1;
    }

    if(stoi(argv[2]) < 0) {
        cerr << "[Max MAX_ZOOM] must be > 0" << endl;
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if(checkArgs(argc, argv)) {
        return 1;
    }

    unsigned long numberIterations = 100;
    long double zoom = 1;
    Complex center(0,0);

    if(argc > 1) {
        numberIterations = stoul(argv[1]);
        zoom = stold(argv[2]);
        center = Complex(stold(argv[3]), stold(argv[4]));
    }

    cout << setprecision(10) << fixed; //change this to be dependent on zoom 
    cout << "===== Mandelbrot Set Generator =====" << endl;
    cout << endl;
    cout << "Image Size: (" << IMAGE_WIDTH << ", " << IMAGE_HEIGHT << ")" << endl;
    cout << "Center Coordinate: (" << center.getReal() << ", " << center.getImag() << ')' << endl;
    cout << "Zoom: " << zoom << endl;
    cout << endl;
    cout << "------------------------------------" << endl;
    cout << endl;

    ofstream fout;
    fout.open("Recursions.ppm");
    if(!fout) {
        cout << "Could not open file." << endl;
        return 0;
    }

    fout << "P2" << endl;
    fout << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << endl;
    fout << numberIterations << endl;
    long double imageMin = ((IMAGE_WIDTH < IMAGE_HEIGHT) ? IMAGE_WIDTH : IMAGE_HEIGHT);

    long double maxValue = 2.125/zoom;
    long double realChange = IMAGE_WIDTH/imageMin;
    long double imagChange = IMAGE_HEIGHT/imageMin;
    
    Complex bottomLeft = Complex(
        center.getReal() - (maxValue)*realChange, 
        center.getImag() - (maxValue)*imagChange
    );

    Complex coords;
    for (int graphI = 0; graphI < IMAGE_HEIGHT; graphI++) {
        for (int graphX = 0; graphX < IMAGE_WIDTH; graphX++, fout << ' ') {
            coords = bottomLeft + Complex(((2*maxValue*graphX)/(IMAGE_WIDTH-1))*realChange, ((2*maxValue*graphI)/(IMAGE_HEIGHT-1))*imagChange);
            fout << mandelbrotEscape(coords, numberIterations);
        }
        fout << endl;
    }
    fout.close();
}