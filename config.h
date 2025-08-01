#ifndef CONFIG_H
#define CONFIG_H

// Variables for Image Processing
//  - Start Values
constexpr unsigned int START_IMAGE_WIDTH = 1920;
constexpr unsigned int START_IMAGE_HEIGHT = 1080;
constexpr double START_CENTER_X = 0;
constexpr double START_CENTER_I = 0;
constexpr double START_ZOOM = 1;
constexpr unsigned int START_ITERATIONS = 40;

//  - Max Values
constexpr unsigned int MAX_IMAGE_WIDTH  = 3024;
constexpr unsigned int MAX_IMAGE_HEIGHT = 1964;


// Variables for Mandelbrot Calculation
//  - Sets Max Variable size to 2.125
constexpr int IMAGE_RADIUS_NUMERATOR = 17; 
constexpr int IMAGE_RADIUS_DENOMINATOR = 8;

//  - Max Values
constexpr int MAX_ITERATIONS = 10000;
constexpr int MAX_ZOOM = 10000000;
#endif