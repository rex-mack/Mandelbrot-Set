#ifndef MANDELBROT_GRAPH_HPP
#define MANDELBROT_GRAPH_HPP

#include "complex.hpp"

class MandelbrotGraph {
    private:
    unsigned int imageWidth;
    unsigned int imageHeight;
    Complex center;
    double zoom;
    unsigned int totalIterations;
    Complex *coordinates; //starting coordinates
    Complex *escapeCoordinates; //where coordiantes ended up
    unsigned int *escapeIterations; //how many recursions that point has gone through
    
    void setImageCoordinates();

    public:
    MandelbrotGraph(unsigned int setImageWidth, unsigned int setImageHeight, Complex setCenter, double setZoom);
    ~MandelbrotGraph();

    void setIterationDepth(unsigned int targetIterations);

    Complex getCenter() const;
    double getZoom() const;
    unsigned int getImageWidth() const;
    unsigned int getImageHeight() const;
    unsigned int getMaxIterations() const;
    Complex getCoordinates(unsigned int imageX, unsigned int imageY) const; // image (0,0) is top left
    Complex getEscapeCoordinates(unsigned int imageX, unsigned int imageY) const; // image (0,0) is top left
    unsigned int getEscapeIteration(unsigned int imageX, unsigned int imageY) const; // image (0,0) is top left
};

#endif