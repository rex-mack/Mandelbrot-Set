#ifndef MANDELBROT_GRAPH_HPP
#define MANDELBROT_GRAPH_HPP

#include "complex.hpp"

class MandelbrotGraph {
    private:
    unsigned int imageWidth;
    unsigned int imageHeight;
    Complex center;
    double zoom;
    unsigned int targetIterations;
    unsigned int maxIterations;
    unsigned int minIterations;
    Complex *coordinates; //starting coordinates
    Complex *escape; //where coordiantes ended up
    unsigned int *iterations; //how many recursions that point has gone through

    void validateValues(Complex center, double zoom, unsigned int targetIterations, unsigned int imageWidth, unsigned int imageHeight) const; // validates the values

    void setImageCoordinates(); //sets the coordinates for each point
    void runMandelbrotIterations(); // does the Mandelbrot iterations for each point

    //debug functions
    Complex getCoordinates(unsigned int xPos, unsigned int yPox) const;
    Complex getEscape(unsigned int xPos, unsigned int yPox) const;

    public:
    MandelbrotGraph();
    MandelbrotGraph(unsigned int width, unsigned int height, Complex center, double zoom = 1.0, unsigned int maxIterations = 40);
    ~MandelbrotGraph();

    Complex getCenter() const;
    double getZoom() const;
    unsigned int getTargetIterations() const;
    unsigned int getMaxIterations() const;
    unsigned int getMinIterations() const;
    unsigned int getIterations(unsigned int posX, unsigned int posY) const;
    unsigned int getImageWidth() const;
    unsigned int getImageHeight() const;

    void setGraphValues(Complex center, double zoom, unsigned int targetIterations, unsigned int width, unsigned int height);

    void moveCenterByPixel(int centerX, int centerY);
    void zoomIn(double factor);
    void increaseIterations(unsigned int factor);

    void writeDebugFiles(); // writes debug files to debug_files folder
};

#endif