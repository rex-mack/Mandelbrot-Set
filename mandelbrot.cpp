#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "mandelbrot.hpp"

using namespace std;

/*
 ----- Section -----
*/



/*
 ----- MandelbrotGraph -----
 -- Private --
*/


void MandelbrotGraph::setImageCoordinates() {
    /*
    sets the coordinates for each point
    resets escape, escapeRecursions, & totalIterations to 0
    */

    //precompute the x coordinates and y coordiants
    double coordinatesX[imageWidth];
    double coordinatesI[imageHeight];

    //      precalculate for coordiantes
    double imageMin = std::min(imageWidth, imageHeight);
    double aspectX = static_cast<double>(imageWidth) / imageMin;
    double aspectI = static_cast<double>(imageHeight) / imageMin;
    double maxValue = IMAGE_RADIUS_NUMERATOR / zoom / IMAGE_RADIUS_DENOMINATOR;

    double spanScaleX = (2.0 * maxValue * aspectX) / (imageWidth - 1);
    double spanScaleI = (2.0 * maxValue * aspectI) / (imageHeight - 1);

    //      calculate x coordiantes
    double centerReal = center.getReal();
    for (int x = 0; x < imageWidth; ++x) {
        coordinatesX[x] = centerReal + (x * spanScaleX - maxValue * aspectX);
    }

    //      calculate i coordinates
    double centerImag = center.getImag();
    for (int i = 0; i < imageHeight; ++i) {
        coordinatesI[i] = centerImag + (i * spanScaleI - maxValue * aspectI);
    }

    //insert coordinates & reset everything else to 0
    totalIterations = 0;
    for (int i = 0; i < imageHeight; ++i) {
        for (int x = 0; x < imageWidth; ++x) {
            coordinates[i * imageWidth + x] = Complex(coordinatesX[x], coordinatesI[i]);
            escapeCoordinates[i*imageWidth + x] = Complex();
            escapeIterations[i*imageWidth + x] = 0;
        }
    }
}

/*
 ----- MandelbrotGraph -----
 -- Public --
*/

MandelbrotGraph::MandelbrotGraph(unsigned int setImageWidth, unsigned int setImageHeight, Complex setCenter, double setZoom) {
    if (setImageWidth > MAX_IMAGE_WIDTH) {
        throw std::out_of_range("Image width exceeds maximum allowed size");
    }
    if (setImageHeight > MAX_IMAGE_HEIGHT) {
        throw std::out_of_range("Image height exceeds maximum allowed size");
    }
    if (setImageWidth == 0 || setImageHeight == 0) {
        throw std::invalid_argument("Image dimensions cannot be zero");
    }
    if (setZoom < 1) {
        throw std::invalid_argument("Zoom cannot be < 1");
    }

    imageWidth = setImageWidth;
    imageHeight = setImageHeight;
    center = setCenter;
    zoom = setZoom;

    coordinates = new Complex[setImageWidth * setImageHeight];
    escapeCoordinates = new Complex[setImageWidth * setImageHeight];
    escapeIterations = new unsigned int[setImageWidth * setImageHeight];

    setImageCoordinates();
}


MandelbrotGraph::~MandelbrotGraph() {
    delete[] coordinates;
    delete[] escapeCoordinates;
    delete[] escapeIterations;
}


void MandelbrotGraph::setIterationDepth(unsigned int targetIterations) {
    if (targetIterations < totalIterations) setImageCoordinates();
        
    for(int i = 0; i < imageHeight; ++i) {
        for(int x = 0; x < imageWidth; ++x) {
            unsigned int pixelIndex = i*imageWidth + x;
            if (escapeIterations[pixelIndex] == totalIterations) {
                for (int iter = totalIterations; (iter < targetIterations) && (escapeCoordinates[pixelIndex].magnitudeSquared() <= 4.0); ++iter) {
                    escapeCoordinates[pixelIndex] = escapeCoordinates[pixelIndex].square() + coordinates[pixelIndex];
                    ++escapeIterations[pixelIndex];
                }
            }
        }
    }

    totalIterations = targetIterations;
}

Complex MandelbrotGraph::getCenter() const {
    return center;
}

double MandelbrotGraph::getZoom() const {
    return zoom;
}

unsigned int MandelbrotGraph::getImageWidth() const {
    return imageWidth;
}

unsigned int MandelbrotGraph::getImageHeight() const {
    return imageHeight;
}

unsigned int MandelbrotGraph::getMaxIterations() const {
    return totalIterations;
}

Complex MandelbrotGraph::getCoordinates(unsigned int imageX, unsigned int imageY) const {
    return coordinates[imageY * imageWidth + imageX];
}
Complex MandelbrotGraph::getEscapeCoordinates(unsigned int imageX, unsigned int imageY) const {
    return escapeCoordinates[imageY * imageWidth + imageX];
}
unsigned int MandelbrotGraph::getEscapeIteration(unsigned int imageX, unsigned int imageY) const {
    return escapeIterations[imageY * imageWidth + imageX];
}