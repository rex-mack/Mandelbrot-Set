#include <stdexcept>
#include <algorithm>
#include "mandelbrot.hpp"

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
    double maxValue = 17.0 / 8.0 / zoom;

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
            escape[i*imageWidth + x] = Complex();
            escapeIterations[i*imageWidth + x] = 0;
        }
    }
}

/*
 ----- MandelbrotGraph -----
 -- Public --
*/

MandelbrotGraph::MandelbrotGraph(unsigned int setImageWidth, unsigned int setImageHeight, Complex setCenter, unsigned int setZoom) {
    if (setImageWidth > MAX_IMAGE_WIDTH) {
        throw std::out_of_range("Image width exceeds maximum allowed size");
    }
    if (setImageHeight > MAX_IMAGE_HEIGHT) {
        throw std::out_of_range("Image height exceeds maximum allowed size");
    }
    if (setImageWidth == 0 || setImageHeight == 0) {
        throw std::invalid_argument("Image dimensions cannot be zero");
    }

    imageWidth = setImageWidth;
    imageHeight = setImageHeight;
    center = setCenter;
    zoom = setZoom;

    setImageCoordinates();
}

void MandelbrotGraph::setIterationDepth(unsigned int targetIterations) {
    if (targetIterations < totalIterations) setImageCoordinates();
        
    for(int i = 0; i < imageHeight; ++i) {
        for(int x = 0; x < imageWidth; ++x) {
            unsigned int pixelIndex = i*imageWidth + x;
            if (escapeIterations[pixelIndex] >= totalIterations)
                continue;

            for (int iter = totalIterations; (iter < targetIterations) && escape[pixelIndex].magnitudeSquared() <= 4.0; ++iter) {
                escape[pixelIndex] = escape[pixelIndex].square() + coordinates[pixelIndex];
                ++escapeIterations[pixelIndex];
            }
        }
    }

    totalIterations = targetIterations;
}