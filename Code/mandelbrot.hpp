#ifndef MANDELBROT_GRAPH_HPP
#define MANDELBROT_GRAPH_HPP

#include "complex.hpp"

class MandelbrotGraph {
    private:
    unsigned int imageWidth;
    unsigned int imageHeight;
    Complex center;
    double zoom;
    unsigned int maxIterations;
    unsigned int minIterations;
    Complex *coordinates; //starting coordinates
    Complex *escape; //where coordiantes ended up
    unsigned int *iterations; //how many recursions that point has gone through
    
    void setImageCoordinates();

    public:
    MandelbrotGraph(unsigned int setImageWidth, unsigned int setImageHeight, Complex setCenter, double setZoom);
    ~MandelbrotGraph();

    void setIterationDepth(unsigned int targetIterations);
    void move(int xPixels, int yPixels); //move image Right / Up so many pixels
    void changeZoom(double setZoom); //set zoom
    void changeImageSize(unsigned int setWidth, unsigned int setHeight);

    Complex getCenter() const;
    double getZoom() const;
    unsigned int getImageWidth() const;
    unsigned int getImageHeight() const;
    unsigned int getMinIterations() const;
    unsigned int getMaxIterations() const;
    Complex getCoordinates(unsigned int imageX, unsigned int imageY) const; // image (0,0) is top left
    Complex getEscape(unsigned int imageX, unsigned int imageY) const; // image (0,0) is top left
    unsigned int getIterations(unsigned int imageX, unsigned int imageY) const; // image (0,0) is top left
};

#endif