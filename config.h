#ifndef CONFIG_H
#define CONFIG_H

// Max Image Dimensions
constexpr unsigned int MAX_IMAGE_WIDTH  = 3024;
constexpr unsigned int MAX_IMAGE_HEIGHT = 1964;

// Image Variables
constexpr int IMAGE_WIDTH  = 1920;
constexpr int IMAGE_HEIGHT = 1080;
    //sets radius for 2.125
constexpr int IMAGE_RADIUS_NUMERATOR = 17; 
constexpr int IMAGE_RADIUS_DENOMINATOR = 8;

// Variables for Mandelbrot Calculation
constexpr int MAX_ITERATIONS = 100000;
constexpr int MAX_ZOOM = 100000;
#endif