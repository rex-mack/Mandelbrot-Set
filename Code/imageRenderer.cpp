#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "mandelbrot.hpp"
#include "parseString.hpp"

void printImage(const MandelbrotGraph &graph) {
    std::cout << " --- Rendering Mandelbrot Set Image -----\n";

    //create .ppm file
    std::ofstream out("Mandelbrot_Image.pgm");
    if (!out) throw std::runtime_error("Failed to open Mandelbrot_Image.pgm");

    std::cout << "📝 Writing file to ./Mandelbrot_Image.pgm\n";

    //header variables
    const Complex center = graph.getCenter();
    const double zoom = graph.getZoom();
    const unsigned int width  = graph.getImageWidth();
    const unsigned int height = graph.getImageHeight();
    const unsigned int targetIterations = graph.getTargetIterations();
    const unsigned int maxIterations = graph.getMaxIterations();
    const unsigned int minIterations = graph.getMinIterations();

    //Write the PPM header
    out << "P2\n"; // P2 format for grayscale
    out << " # Mandelbrot grayscale map\n";
    out << " # Center Coordinates: " << center << '\n';
    out << " # Zoom: " << zoom << '\n';
    out << " # Target Iterations: " << targetIterations << '\n';
    out << " # Iterations Min & Max: " << minIterations << " - " << maxIterations << '\n';
    out << width << " " << height << '\n';
    out << 255 << '\n';

    // Write the pixel data
    unsigned int iterationsDiff = maxIterations - minIterations;
    for (unsigned int pixY = 0; pixY < height; ++pixY) {
        if (pixY > 0) {
            out << '\n';
        }
        for (unsigned int pixX = 0; pixX < width; ++pixX) {
            if (pixX > 0) {
                out << " ";
            }
            unsigned int iterations = graph.getIterations(pixX, pixY);
            unsigned int colorValue = (iterations - minIterations) * 255 / iterationsDiff; // Normalize to 0-255
            out << colorValue;
        }
    }

    std::cout << "  ✅ Image written to ./Mandelbrot_Image.pgm\n";
    out.close();

    std::cout << " ------------------------------------\n";
}
