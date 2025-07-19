#include "config.h"
#include "complex.cpp"

class MandelbrotGraph {
    private:
    unsigned int imageWidth;
    unsigned int imageHeight;
    Complex center;
    unsigned int zoom;
    unsigned int totalIterations;
    Complex coordinates[MAX_IMAGE_HEIGHT * MAX_IMAGE_WIDTH]; //starting coordinates
    Complex escape[MAX_IMAGE_HEIGHT * MAX_IMAGE_WIDTH]; //where coordiantes ended up
    unsigned int escapeIterations[MAX_IMAGE_HEIGHT * MAX_IMAGE_WIDTH]; //how many recursions that point has gone through
    
    void setImageCoordinates();

    public:
    MandelbrotGraph(unsigned int setImageWidth, unsigned int setImageHeight, Complex setCenter, unsigned int setZoom);

    void setIterationDepth(unsigned int targetIterations);
};