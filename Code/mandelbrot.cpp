#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "mandelbrot.hpp"
#include "config.h"

using namespace std;

/*
 ----- MandelbrotGraph -----
 -- Private --
*/

void MandelbrotGraph::validateValues(Complex center, double zoom, unsigned int targetIterations, unsigned int imageWidth, unsigned int imageHeight) const {
    double maxRadius = IMAGE_RADIUS_NUMERATOR / IMAGE_RADIUS_DENOMINATOR;
    if (center.getReal() < -maxRadius || center.getReal() > maxRadius||
        center.getImag() < -maxRadius || center.getImag() > maxRadius) {
        throw std::invalid_argument("Center coordinates must be within the range [-" + std::to_string(maxRadius) + ", " + std::to_string(maxRadius) + "]");
    }
    if (zoom < 1) {
        throw std::invalid_argument("Zoom level must be greater than 1");
    } else if (zoom > MAX_ZOOM) {
        throw std::invalid_argument("Zoom level must not exceed " + std::to_string(MAX_ZOOM));
    }
    if (imageWidth == 0 || imageHeight == 0) {
        throw std::invalid_argument("Image dimensions cannot be zero");
    } else if (imageWidth > MAX_IMAGE_WIDTH || imageHeight > MAX_IMAGE_HEIGHT) {
        throw std::invalid_argument("Image dimensions must not exceed " + std::to_string(MAX_IMAGE_WIDTH) + "x" + std::to_string(MAX_IMAGE_HEIGHT));
    }
    if (targetIterations > MAX_ITERATIONS) {
        throw std::invalid_argument("Maximum iterations must not exceed " + std::to_string(MAX_ITERATIONS));
    }
}
void MandelbrotGraph::setImageCoordinates() {
    /*
    sets the coordinates for each point
    resets escape, escapeRecursions, minIterations & maxIterations
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

    //insert coordinates & reset escape and iterations
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
}
void MandelbrotGraph::runMandelbrotIterations() { //implement doing this in parallel
    /*
    does the Mandelbrot iterations for each point
     - sets escape, escapeRecursions, maxIterations, minIterations
    */

    //check if the targetIterations is less than maxIterations
    if (targetIterations < maxIterations) setImageCoordinates();

    std::cout << "🚀 Doing at least " << (targetIterations - maxIterations) << " Mandelbrot iterations for each " << imageWidth * imageHeight << " pixels\n";

    //set escape and iterations
    for(unsigned int i = 0; i < imageHeight / 2; ++i) {
        unsigned int pixelIndexY = i * imageWidth;
        for (unsigned int x = 0; x < imageWidth; ++x) {
            unsigned int pixelIndex = pixelIndexY + x;
            while(iterations[pixelIndex] < targetIterations && escape[pixelIndex].magnitudeSquared() <= 4.0) {
                escape[pixelIndex] = escape[pixelIndex] * escape[pixelIndex] + coordinates[pixelIndex];
                ++iterations[pixelIndex];
            }
        }
    }
    std::cout << " ⏳ Halfway through iterations...\n";
    for(unsigned int i = imageHeight / 2; i < imageHeight; ++i) {
        for (unsigned int x = 0; x < imageWidth; ++x) {
            unsigned int pixelIndex = i * imageWidth + x;
            while(iterations[pixelIndex] < targetIterations && escape[pixelIndex].magnitudeSquared() <= 4.0) {
                escape[pixelIndex] = escape[pixelIndex] * escape[pixelIndex] + coordinates[pixelIndex];
                ++iterations[pixelIndex];
            }
        }
    }
    std::cout << " ✅ Mandelbrot iterations complete.\n";

    //set maxIterations and minIterations
    unsigned int maxPos = imageHeight * imageWidth;
    maxIterations = 0;
    minIterations = targetIterations; //set minIterations to targetIterations, so it will be updated if any pixel has less iterations
    for(unsigned int i = 0; i < maxPos; ++i) {
        if (maxIterations < iterations[i]) maxIterations = iterations[i];
        if (minIterations > iterations[i]) minIterations = iterations[i];
    }
}

/*
debug funcitons
*/

Complex MandelbrotGraph::getCoordinates(unsigned int xPos, unsigned int yPos) const {
    if(xPos >= imageWidth || yPos >= imageHeight) {
        throw std::out_of_range("Pixel coordinates out of bounds: (" + std::to_string(xPos) + ", " + std::to_string(yPos) + ")");
        return Complex(); // Return a default value in case of error
    }
    return coordinates[yPos * imageWidth + xPos];
}
Complex MandelbrotGraph::getEscape(unsigned int xPos, unsigned int yPos) const {
    if (xPos >= imageWidth || yPos >= imageHeight) {
        throw std::out_of_range("Pixel coordinates out of bounds: (" + std::to_string(xPos) + ", " + std::to_string(yPos) + ")");
        return Complex(); // Return a default value in case of error
    }
    return escape[yPos * imageWidth + xPos];
}

/*
 ----- MandelbrotGraph -----
 -- Public --
*/


MandelbrotGraph::MandelbrotGraph() {
    // Initialize with default values in config.h
    imageWidth = START_IMAGE_WIDTH;
    imageHeight = START_IMAGE_HEIGHT;
    center = Complex(START_CENTER_X, START_CENTER_I);
    zoom = START_ZOOM;
    targetIterations = START_ITERATIONS;

    coordinates = new Complex[imageWidth * imageHeight];
    escape = new Complex[imageWidth * imageHeight];
    iterations = new unsigned int[imageWidth * imageHeight];

    //set the initial coordinates
    setImageCoordinates();
    
    //do the initial Mandelbrot iterations to default maxIterations in config.h
    runMandelbrotIterations();
}
MandelbrotGraph::MandelbrotGraph(unsigned int width, unsigned int height, Complex center, double zoom, unsigned int targetIterations) {
    try {
        validateValues(center, zoom, targetIterations, width, height);
    } catch (const std::invalid_argument &e) {
        throw std::invalid_argument("Invalid parameters: " + std::string(e.what()));
    }
    
    // Initialize with provided values
    imageWidth = width;
    imageHeight = height;
    this->center = center;
    this->zoom = zoom;
    this->targetIterations = targetIterations;

    coordinates = new Complex[imageWidth * imageHeight];
    escape = new Complex[imageWidth * imageHeight];
    iterations = new unsigned int[imageWidth * imageHeight];

    //set the initial coordinates
    setImageCoordinates();
    
    //do the initial Mandelbrot iterations to default maxIterations in config.h
    runMandelbrotIterations();
}
MandelbrotGraph::~MandelbrotGraph() {
    delete[] coordinates; coordinates = nullptr;
    delete[] escape; escape = nullptr;
    delete[] iterations; iterations = nullptr;
}

Complex MandelbrotGraph::getCenter() const {
    return center;
}
double MandelbrotGraph::getZoom() const {
    return zoom;
}
unsigned int MandelbrotGraph::getTargetIterations() const {
    return targetIterations;
}
unsigned int MandelbrotGraph::getMaxIterations() const {
    return maxIterations;
}
unsigned int MandelbrotGraph::getMinIterations() const {
    return minIterations;
}
unsigned int MandelbrotGraph::getIterations(unsigned int posX, unsigned int posY) const {
    if(posX >= imageWidth || posY >= imageHeight) {
        throw std::out_of_range("Pixel coordinates out of bounds: (" + std::to_string(posX) + ", " + std::to_string(posY) + ")");
        return 0; // Return a default value in case of error
    }
    return iterations[posY * imageWidth + posX];
}
unsigned int MandelbrotGraph::getImageWidth() const {
    return imageWidth;
}
unsigned int MandelbrotGraph::getImageHeight() const {
    return imageHeight;
}

void MandelbrotGraph::setGraphValues(Complex center, double zoom, unsigned int targetIterations, unsigned int imageWidth, unsigned int imageHeight) {
    /*
    sets the graph values
     - sets the center, zoom, targetIterations, imageWidth, imageHeight
     - sets the coordinates and does the Mandelbrot iterations
    */

    // Validate the input values
    try {
        validateValues(center, zoom, targetIterations, imageWidth, imageHeight);
    } catch (const std::invalid_argument &e) {
        throw std::invalid_argument("Invalid parameters: " + std::string(e.what()));
    }

    //if nothing has changed, just return
    if (this->center == center && this->zoom == zoom && this->targetIterations == targetIterations &&
        this->imageWidth == imageWidth && this->imageHeight == imageHeight) {
        return;
    }

    this->targetIterations = targetIterations;

    //if nothing has changed except the targetIterations, just do the iterations
    if (this->center == center && this->zoom == zoom &&
        this->imageWidth == imageWidth && this->imageHeight == imageHeight) {
        runMandelbrotIterations();
        return;
    }

    //set the graph values
    this->center = center;
    this->zoom = zoom;

    if(imageWidth != this->imageWidth || imageHeight != this->imageHeight) {
        this->imageWidth = imageWidth;
        this->imageHeight = imageHeight;

        //delete old coordinates and allocate new ones
        delete[] coordinates; coordinates = new Complex[imageWidth * imageHeight];
        delete[] escape; escape = new Complex[imageWidth * imageHeight];
        delete[] iterations; iterations = new unsigned int[imageWidth * imageHeight];
    }

    //set the coordinates & do the iterations
    setImageCoordinates();
    runMandelbrotIterations();

}

void MandelbrotGraph::moveCenterByPixel(int xPixels, int yPixels) {
    //compute new center coordinate X & I
    double imageMin = std::min(imageWidth, imageHeight);
    double aspectX = static_cast<double>(imageWidth) / imageMin;
    double aspectI = static_cast<double>(imageHeight) / imageMin;
    double maxValue = IMAGE_RADIUS_NUMERATOR / zoom / IMAGE_RADIUS_DENOMINATOR;

    double spanScaleX = (2.0 * maxValue * aspectX) / (imageWidth - 1);
    double spanScaleI = (2.0 * maxValue * aspectI) / (imageHeight - 1);

    Complex newCenter = center + Complex(xPixels * spanScaleX, - yPixels * spanScaleI);

    //set new center
    center = newCenter;

    //set coordinates and do Mandelbrot Iterations
    setImageCoordinates();
    runMandelbrotIterations();
}
void MandelbrotGraph::zoomIn(double factor) {
    //set zoom
    zoom += factor;
    if (zoom < 1) {
        throw std::invalid_argument("Zoom level must be greater than 1\n    Zoom set to 1");
        zoom = 1;
    } else if (zoom > MAX_ZOOM) {
        throw std::invalid_argument("Zoom level must not exceed " + std::to_string(MAX_ZOOM) + "\n    Zoom set to " + std::to_string(MAX_ZOOM));
        zoom = MAX_ZOOM;
    }

    //set coordinates & do Mandelbrot Iterations
    setImageCoordinates();
    runMandelbrotIterations();
}
void MandelbrotGraph::increaseIterations(unsigned int factor) {
    //increase maxIterations
    targetIterations += factor;

    //do Mandelbrot Iterations
    runMandelbrotIterations();
}

void MandelbrotGraph::writeDebugFiles() {
    //create files
    string base = "debug_files/";

    if (!std::filesystem::exists(base)) { //if the debug_files folder does not exist, create it
        std::filesystem::create_directory(base);
    }

    string debugCoordinatesFile = base + "debug_coordinates.txt";
    string debugEscapeFile = base + "debug_escape.txt";
    string debugIterationsFile = base + "debug_iterations.pgm";
    unsigned int numberOfDebugs = 0; // This should be a class member or static variable to keep track of debug files
    while (std::filesystem::exists(debugCoordinatesFile)) {
        debugCoordinatesFile = base + "debug_coordinates(" + std::to_string(numberOfDebugs) + ").txt";
        debugEscapeFile = base + "debug_escape(" + std::to_string(numberOfDebugs) + ").txt";
        debugIterationsFile = base + "debug_iterations(" + std::to_string(numberOfDebugs) + ").pgm";
        numberOfDebugs++;
    }
    std::ofstream outCoordinates(debugCoordinatesFile);
    if (!outCoordinates) throw std::runtime_error("Failed to open " + debugCoordinatesFile);
    std::ofstream outEscape(debugEscapeFile);
    if (!outEscape) throw std::runtime_error("Failed to open " + debugEscapeFile);
    std::ofstream outIterations(debugIterationsFile);
    if (!outIterations) throw std::runtime_error("Failed to open " + debugIterationsFile);
    
    numberOfDebugs++; //increase for next debug

    std::cout << "📝 Writing debug files to ./debug_files/\n";

    //header for outCoordinates
    outCoordinates << "P2\n";
    outCoordinates << " # Debug coordinates map\n";
    outCoordinates << " # Center Coordinates: " << center << '\n';
    outCoordinates << " # Zoom: " << zoom << '\n';
    outCoordinates << " # Image Dimensions: " << imageWidth << " " << imageHeight<< '\n';

    //header for outEscape
    outEscape << "P2\n";
    outEscape << " # Debug escape map\n";
    outEscape << " # Center Coordinates: " << center << '\n';
    outEscape << " # Zoom: " << zoom << '\n';
    outEscape << " # Image Dimensions: " << imageWidth << " " << imageHeight << '\n';

    //header for outIterations
    outIterations << "P2\n";
    outIterations << " # Debug iterations map\n";
    outIterations << " # Center Coordinates: " << center << '\n';
    outIterations << " # Zoom: " << zoom << '\n';
    outIterations << imageWidth << " " << imageHeight << '\n';
    outIterations << maxIterations << '\n';

    //image body
    for (unsigned int pixY = 0; pixY < imageWidth; ++pixY) {
        if (pixY > 0) {
            outCoordinates << '\n';
            outEscape << '\n';
            outIterations << '\n';
        }
        for (unsigned int pixX = 0; pixX < imageHeight; ++pixX) {
            if (pixX > 0) {
                outCoordinates << " ";
                outEscape << " ";
                outIterations << " ";
            }
            outCoordinates << getCoordinates(pixX, pixY);
            outEscape << getEscape(pixX, pixY);
            outIterations << getIterations(pixX, pixY);
        }
    }

    std::cout << "  ✅ Debug files written to ./debug_files/\n";
}