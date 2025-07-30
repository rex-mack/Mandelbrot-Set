#ifndef IMAGE_RENDERER_HPP
#define IMAGE_RENDERER_HPP

#include <string>
#include "mandelbrot.hpp"

using namespace std;

class ImageRenderer {
    private:
        unsigned int numberOfDebugs;
    
    public:
        ImageRenderer();

        void writeDebugFiles(const MandelbrotGraph &graph); //prints debug into debug_files

        void printPPM(const MandelbrotGraph &graph);
};

#endif