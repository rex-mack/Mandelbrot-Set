#include <iostream>
#include <fstream>
#include "config.h"
#include "complex.hpp"

using namespace std;

unsigned int mandelbrotEscape(Complex coords, unsigned int iterations) {
    Complex nthIteration;
    unsigned int i = 0;
    while ((i < iterations) && (nthIteration.magnitudeSquared() <= 4)) {
        nthIteration = nthIteration.square() + coords;
        i++;
    }
    return i;
}

int main() {
    int imageSize[2] = {IMAGE_WIDTH,IMAGE_HEIGHT};
    unsigned int numberIterations = 50;
    long double zoom = 10;
    Complex center(-1,0);


    cout << setprecision(10) << fixed; //change this to be dependent on zoom 
    cout << "===== Mandelbrot Set Generator =====" << endl;
    cout << endl;
    cout << "Image Size: (" << imageSize[0] << ", " << imageSize[1] << ")" << endl;
    cout << "Center Coordinate: (" << center.getReal() << ", " << center.getImag() << ')' << endl;
    cout << "Zoom: " << zoom << endl;
    cout << endl;
    cout << "------------------------------------" << endl;
    cout << endl;

    if (zoom < 0) {
        cout << "ERROR: attempted to create graph with zoom: " << zoom << endl;
        cout << "Setting zoom to 0" << endl;
        zoom = 0.0;
        cout << endl;
        cout << "------------------------------------" << endl;
        cout << endl;
    }

    ofstream fout;
    fout.open("Image.ppm");
    if(!fout) {
        cout << "Could not open file." << endl;
        return 0;
    }

    fout << "P2" << endl;
    fout << imageSize[0] << ' ' << imageSize[1] << endl;
    fout << numberIterations << endl;
    int imageMin = ((IMAGE_WIDTH < IMAGE_HEIGHT) ? IMAGE_WIDTH : IMAGE_HEIGHT);

    long double maxValue = 2.5/zoom;
    long double realChange = IMAGE_WIDTH/imageMin;
    long double imagChange = IMAGE_HEIGHT/imageMin;
    
    Complex bottomLeft = Complex(
        center.getReal() - (maxValue)*realChange, 
        center.getImag() - (maxValue)*imagChange
    );

    Complex coords;
    for (int graphI = 0; graphI < IMAGE_HEIGHT; graphI++) {
        for (int graphX = 0; graphX < IMAGE_WIDTH; graphX++, fout << ' ') {
            coords = bottomLeft + Complex((2*maxValue*graphX)/(IMAGE_WIDTH-1)*realChange, (2*maxValue*graphI)/(IMAGE_HEIGHT-1)*imagChange);
            fout << mandelbrotEscape(coords, numberIterations);
        }
        fout << endl;
    }

    fout.close();
}