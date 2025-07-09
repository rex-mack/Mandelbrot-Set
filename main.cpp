#include <iostream>
#include <fstream>
#include "config.h"
#include "types.hpp"

using namespace std;

int main() {
    cout << setprecision(10) << fixed;

    int imageSize[2] = {IMAGE_WIDTH,IMAGE_HEIGHT};
    int numberIterations = 1;
    long double zoom = 1;
    Complex center(0.0,0.0);
    
    cout << "===== Mandelbrot Set Generator =====" << endl;
    cout << endl;
    cout << "Image Size: (" << imageSize[0] << ", " << imageSize[1] << ")" << endl;
    cout << "Center Coordinate: (" << center << ')' << endl;
    cout << "Zoom: " << zoom << endl;
    cout << endl;
    cout << "------------------------------------" << endl;
    cout << endl;
    cout << "Pixel Size: " << endl;
    cout << endl;
    cout << "====================================" << endl;

}