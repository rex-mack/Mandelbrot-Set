#include <iostream>
#include <fstream>
#include "config.h"
#include "types.hpp"

using namespace std;

int main() {
    int imageSize[2] = {IMAGE_WIDTH,IMAGE_HEIGHT};
    int numberIterations = 1;
    long double zoom = 1;
    Complex center(0,0);


    cout << setprecision(10) << fixed; //change this to be dependent on zoom 
    cout << "===== Mandelbrot Set Generator =====" << endl;
    cout << endl;
    cout << "Image Size: (" << imageSize[0] << ", " << imageSize[1] << ")" << endl;
    cout << "Center Coordinate: (" << center << ')' << endl;
    cout << "Zoom: " << zoom << endl;
    cout << endl;
    cout << "------------------------------------" << endl;

    //create graph
    Complex graph[imageSize[0]][imageSize[1]];

    //apply coordinates
    /*
    at least have (-5,5) and (5, -5) coordinates in zoom 1, center(0,0)
    */

    //get distance between each pixel
    int fracNumer = 5;
    int fracDenom = (imageSize[0]>imageSize[1]) ? imageSize[1] : imageSize[0];

    


    for (int posX = 0; posX < imageSize[0]; posX++) {
        for (int posY = 0; posY < imageSize[1]; posY++) {
            graph[posX][posY] = Complex();
        }
    }
    //do mandelbrot recursion on number

    //apply color
    //print picture

}