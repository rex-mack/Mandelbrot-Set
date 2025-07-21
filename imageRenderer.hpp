#ifndef IMAGE_RENDERER_HPP
#define IMAGE_RENDERER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include "mandelbrot.hpp"

using namespace std;

class ImageRenderer {
    private:
    
    public:
        void writeDebugFiles(const MandelbrotGraph &graph);
};

#endif