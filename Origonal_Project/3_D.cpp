/*
CSCI 1300 Summer 2022
Author: RepixX Mack
Recitation: 301 – TA Tuhina Tripathi
Project - 3D
*/

#include <iostream>
using namespace std;
#include <fstream>
#include <cmath>

void squareComplexNumber(long double coords[2]) {
    long double calc = coords[0]*coords[0] - coords[1]*coords[1]; // x part
    coords[1] = 2*coords[0]*coords[1]; // i part & putting it into i part
    coords[0] = calc; // putting int into x part;
}

void cubeComplexNumber(long double coords[2]) {
    long double calc = coords[0]*(coords[0]*coords[0] - 3*coords[1]*coords[1]); // x part
    coords[1] = coords[1]*(3*coords[0]*coords[0]-coords[1]*coords[1]); // i part & putting it into i part
    coords[0] = calc; // putting int into x part;
}

void mandelbrotEquasion(long double coordinates[2], long double returnCoordinates[2]) {
    squareComplexNumber(returnCoordinates);
    returnCoordinates[0] += coordinates[0];
    returnCoordinates[1] += coordinates[1];
}

int numOfIterationsTillEscapeMandbrotEq(long double coordinates[2], int maxIteration, long double returnCoordinates[2], int iteration = 0) { // start with iterations = iteration
    if(iteration < maxIteration) {
        if(returnCoordinates[0]*returnCoordinates[0] + returnCoordinates[1]*returnCoordinates[1] <= 4) {
            mandelbrotEquasion(coordinates, returnCoordinates);
            return numOfIterationsTillEscapeMandbrotEq(coordinates, maxIteration, returnCoordinates, iteration+1);
        }
    }
    return iteration;
}

void printColorRed(long double normalizedValue, int colors[3]) {
    colors[0] = int(normalizedValue*255);
    colors[1] = 0;
    colors[2] = 0;
}

void printColorBlueToRed(long double normalizedValue, int colors[3]) {
    colors[0] = int(normalizedValue*255);
    colors[1] = 0;
    colors[2] = int((1-normalizedValue)*255);
}

void printColorA(long double normalizedValue, int colors[3]) {
    int calc = int(255*normalizedValue);
    if(calc <= 240/255.0) {
        colors[0] = ((255-calc)*51)/48;
        colors[1] = calc/4;
        colors[2] = calc/4;
    } else {
        colors[0] = 255;
        colors[1] = (255-calc)*4;
        colors[2] = (255-calc)*4;
    }
}

void printColorB(int numIterations, int colors[3]) {
    colors[0] = int(((numIterations%75)/75.0)*225*2);
    colors[1] = int(((numIterations%180)/180.0)*225*2);
    colors[2] = int((((numIterations%15)/15.0)*255*2));
    if(colors[0] >= 255) {
        colors[0] = (255*2) - colors[0]-10;
    }
    if(colors[1] >= 255) {
        colors[1] = (255*2) - colors[1]-10;
    }
    if(colors[2] >= 255) {
        colors[2] = (255*2) - colors[2]-10;
    }
}

/*
Great Sources:

https://theses.liacs.nl/pdf/2018-2019-JonckheereLSde.pdf
*/

int main() {

     cout << setprecision(10) << fixed;

    /*
    Cool Places:
        maxIterations = 100
        zoom = 100
        center[2] = {-0.451275, -0.56085}
        printColorA(numOfIterations/long double(maxIterations), colors)

        maxIterations = 150;
        zoom = 400;
        center[2] = {-0.443715, -0.56688};
        printColorB(numOfIterations, colors);

        maxIterations = 520;
        zoom = 8000000;
        center[2] = {-0.4453871125, -0.5654119438};
        printColorB(numOfIterations, colors);

    */

    //image stuff
    int imageSize[2] = {1000,1000};
    int maxIterations = 100;
    long double zoom = 100;
    long double center[2] = {-0.451275, -0.56085};
    int findPoint[2] = {-200,100};
    long double pixelSize = 0.0045/zoom; // start -> 0.0045
    cout << "Center Coordinate: (" << center[0] << ", " << center[1] << ')' << endl;
    cout << "Each Pixel is " << pixelSize << " distance appart." << endl;

    //image stuff calculations
    long double corners[2][2];
    corners[0][0] = center[0]-((imageSize[0]/2)*pixelSize);
    corners[0][1] = center[1]+((imageSize[0]/2)*pixelSize);
    corners[1][0] = corners[0][0]+imageSize[0]*pixelSize;
    corners[1][1] = corners[0][1]-imageSize[0]*pixelSize;
    cout << "Image goes from (" << corners[0][0] << ", " << corners[0][1] << ") to (" << corners[1][0] << ", " << corners[1][1] << ")" << endl;



    // open & check file
    ofstream fout;
    fout.open("3D_image.ppm");
    if(!fout) {
        cout << "Could not open file." << endl;
        return 0;
    }

    // Writing into file

    // Output PPM file header
    fout << "P3" << endl;
    fout << imageSize[0] << ' ' << imageSize[1] << endl;
    fout << "255" << endl;

    //drawn in graph style: x by i
    long double complexNumber[2];
    long double returnComplexNumber[2];
    int numOfIterations;
    int colors[3];
    for(int pixY = 0; pixY < imageSize[1]; pixY++) {
        for (int pixX = 0; pixX < imageSize[0]; pixX++) {
            //fout << "(" << pixX << ", " << pixY << ")";
            complexNumber[0] = corners[0][0] + pixelSize*pixX;
            complexNumber[1] = corners[0][1] - pixelSize*pixY;
            //fout << "(" << complexNumber[0] << " + i" << complexNumber[1] << ")";
            returnComplexNumber[0] = 0;
            returnComplexNumber[1] = 0;
            numOfIterations = numOfIterationsTillEscapeMandbrotEq(complexNumber, maxIterations, returnComplexNumber);
            // fout << numOfIterations;
            /*
            if(pixX == imageSize[0]/2 && pixY == imageSize[1]/2) {
                fout << "255 255 255";
            } else if (pixX == (imageSize[0]/2)+findPoint[0]&& pixY == (imageSize[1]/2)-findPoint[1]) {
                fout << "255 255 255";
                cout << "Find Point Coordinate = (" << complexNumber[0] << ", " << complexNumber[1] << ')' << endl;
            } else 
            */
            if(numOfIterations == maxIterations) {
                fout << "0 0 0";
            } else {
                printColorBlueToRed(numOfIterations/double(maxIterations), colors);
                fout << colors[0] << ' ' << colors[1] << ' ' << colors[2];
            }
            
            if(pixX != imageSize[0]-1) { // puts spaces after
                fout << ' ';
            }
        }
        //if(pixY != imageSize[1]-1) { // puts spaces after
            fout << endl;
        //}
    }

    //close file
    fout.close();
    return 0;
}