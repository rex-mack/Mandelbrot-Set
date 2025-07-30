#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "mandelbrot.hpp"

using namespace std;

/*
 ----- MandelbrotGraph -----
 -- Private --
*/


void MandelbrotGraph::setImageCoordinates() {
    /*
    sets the coordinates for each point
    resets escape, escapeRecursions, & maxIterations to 0
    */

    //precompute the x coordinates and y coordiants
    double *coordinatesX = new double [imageWidth];
    double *coordinatesI = new double [imageHeight];

    //      precalculate for coordiantes
    double imageMin = std::min(imageWidth, imageHeight);
    double aspectX = static_cast<double>(imageWidth) / imageMin;
    double aspectI = static_cast<double>(imageHeight) / imageMin;
    double maxValue = IMAGE_RADIUS_NUMERATOR / zoom / IMAGE_RADIUS_DENOMINATOR;

    double spanScaleX = (2.0 * maxValue * aspectX) / (imageWidth - 1);
    double spanScaleI = (2.0 * maxValue * aspectI) / (imageHeight - 1);

    //      calculate x coordiantes
    double centerReal = center.getReal();
    for (unsigned int x = 0; x < imageWidth; ++x) {
        coordinatesX[x] = centerReal + (x * spanScaleX - maxValue * aspectX);
    }

    //      calculate i coordinates
    double centerImag = center.getImag();
    for (unsigned int i = 0; i < imageHeight; ++i) {
        coordinatesI[i] = centerImag + (i * spanScaleI - maxValue * aspectI);
    }

    //insert coordinates & reset everything else to 0
    std::cout << "🔧 Setting coordinates..." << std::endl;
    maxIterations = 0;
    for (unsigned int i = 0; i < imageHeight; ++i) {
        for (unsigned int x = 0; x < imageWidth; ++x) {
            coordinates[i * imageWidth + x] = Complex(coordinatesX[x], coordinatesI[i]);
            escape[i*imageWidth + x] = Complex();
            iterations[i*imageWidth + x] = 0;
        }
    }

    //reset other variables
    maxIterations = 0;
    minIterations = 0;

    //delete used pointers
    delete[] coordinatesX;
    delete[] coordinatesI;

    std::cout << "  ✅ Finished setting coordinates.\n";
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
    escape = new Complex[setImageWidth * setImageHeight];
    iterations = new unsigned int[setImageWidth * setImageHeight];

    setImageCoordinates();
}


MandelbrotGraph::~MandelbrotGraph() {
    delete[] coordinates;
    delete[] escape;
    delete[] iterations;
}

//implement doing this in parallel (race conditions with minIterations)
void MandelbrotGraph::setIterationDepth(unsigned int targetIterations) {
    std::cout << "🚀 Doing " << (targetIterations - maxIterations) << " Mandelbrot iterations for " << imageWidth*imageHeight << " pixels\n ";
    if (targetIterations < maxIterations) setImageCoordinates();
    minIterations = targetIterations;
    for(unsigned int i = 0; i < imageHeight/2; ++i) {
        unsigned int pixelIndexY = i*imageWidth;
        for(unsigned int x = 0; x < imageWidth; ++x) {
            unsigned int pixelIndex = pixelIndexY + x;
            if (iterations[pixelIndex] == maxIterations) {
                for (unsigned int iter = maxIterations; (iter < targetIterations) && (escape[pixelIndex].magnitudeSquared() <= 4.0); ++iter) {
                    escape[pixelIndex] = escape[pixelIndex] * escape[pixelIndex] + coordinates[pixelIndex];
                    ++iterations[pixelIndex];
                }
                if (minIterations > iterations[pixelIndex]) minIterations = iterations[pixelIndex];
            }
        }
    }
    std::cout << " ⏳ Halfway through iterations...\n";
    for(unsigned int i = imageHeight/2; i < imageHeight; ++i) {
        for(unsigned int x = 0; x < imageWidth; ++x) {
            unsigned int pixelIndex = i*imageWidth + x;
            if (iterations[pixelIndex] == maxIterations) {
                for (unsigned int iter = maxIterations; (iter < targetIterations) && (escape[pixelIndex].magnitudeSquared() <= 4.0); ++iter) {
                    escape[pixelIndex] = escape[pixelIndex] * escape[pixelIndex] + coordinates[pixelIndex];
                    ++iterations[pixelIndex];
                }
                if (minIterations > iterations[pixelIndex]) minIterations = iterations[pixelIndex];
            }
        }
    }
    maxIterations = targetIterations;
    std::cout << "  ✅ Mandelbrot iterations complete.\n";
}

void MandelbrotGraph::move(int xPixels, int yPixels) {
    //compute new center coordinate X & I
    double imageMin = std::min(imageWidth, imageHeight);
    double aspectX = static_cast<double>(imageWidth) / imageMin;
    double aspectI = static_cast<double>(imageHeight) / imageMin;
    double maxValue = IMAGE_RADIUS_NUMERATOR / zoom / IMAGE_RADIUS_DENOMINATOR;

    double spanScaleX = (2.0 * maxValue * aspectX) / (imageWidth - 1);
    double spanScaleI = (2.0 * maxValue * aspectI) / (imageHeight - 1);

    Complex newCenter = center + Complex(xPixels * spanScaleX, - yPixels * spanScaleI);
    unsigned int targetIterations = maxIterations; //saves 

    //set new center
    center = newCenter;

    //set coordinates
    setImageCoordinates();

    //do Mandelbrot Iterations
    setIterationDepth(targetIterations);
}

void MandelbrotGraph::changeZoom(double setZoom) {
    //set zoom
    zoom = setZoom;
    unsigned int targetIterations = maxIterations; //saves 

    //set coordinates
    setImageCoordinates();

    //do Mandelbrot Iterations
    setIterationDepth(targetIterations);
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

unsigned int MandelbrotGraph::getMinIterations() const {
    return minIterations;
}
unsigned int MandelbrotGraph::getMaxIterations() const {
    return maxIterations;
}

Complex MandelbrotGraph::getCoordinates(unsigned int imageX, unsigned int imageY) const {
    return coordinates[imageY * imageWidth + imageX];
}
Complex MandelbrotGraph::getEscape(unsigned int imageX, unsigned int imageY) const {
    return escape[imageY * imageWidth + imageX];
}
unsigned int MandelbrotGraph::getIterations(unsigned int imageX, unsigned int imageY) const {
    return iterations[imageY * imageWidth + imageX];
}