#include <filesystem>
#include <fstream>
#include <iostream>
#include "imageRenderer.hpp"

namespace fs = std::filesystem;

/*
 ----- Image Renderer -----
 -- Public --
*/

ImageRenderer::ImageRenderer() {
    //reset debug_files
    numberOfDebugs = 0;
    fs::remove_all("debug_files");
    fs::create_directories("debug_files");
}


//add setprecision to the coordiantes 
void ImageRenderer::writeDebugFiles(const MandelbrotGraph &graph) {
    //create files
    string base = "debug_files/";
    std::ofstream outCoordinates(base + "debug_coordinates(" + std::to_string(numberOfDebugs) + ").txt");
    if (!outCoordinates) throw std::runtime_error("debug_coordinates(" + std::to_string(numberOfDebugs) + ").txt");
    std::ofstream outEscape(base + "debug_escape(" + std::to_string(numberOfDebugs) + ").txt");
    if (!outEscape) throw std::runtime_error("Failed to open debug_escape(" + std::to_string(numberOfDebugs) + ").txt");
    std::ofstream outIterations(base + "debug_iterations(" + std::to_string(numberOfDebugs) + ").pgm");
    if (!outIterations) throw std::runtime_error("Failed to open debug_iterations(" + std::to_string(numberOfDebugs) + ").pgm");

    numberOfDebugs++; //increase for next debug

    std::cout << "📝 Writing debug files to ./debug_files/\n";

    //header variables
    const Complex center = graph.getCenter();
    const double zoom = graph.getZoom();
    const unsigned int width  = graph.getImageWidth();
    const unsigned int height = graph.getImageHeight();

    //header for outCoordinates
    outCoordinates << "P2\n";
    outCoordinates << " # Debug coordinates map\n";
    outCoordinates << " # Center Coordinates: " << center << '\n';
    outCoordinates << " # Zoom: " << zoom << '\n';
    outCoordinates << " # Image Dimensions: " << width << " " << height << '\n';

    //header for outEscape
    outEscape << "P2\n";
    outEscape << " # Debug escape map\n";
    outEscape << " # Center Coordinates: " << center << '\n';
    outEscape << " # Zoom: " << zoom << '\n';
    outCoordinates << " # Image Dimensions: " << width << " " << height << '\n';

    //header for outIterations
    outIterations << "P2\n";
    outIterations << " # Debug iterations map\n";
    outIterations << " # Center Coordinates: " << center << '\n';
    outIterations << " # Zoom: " << zoom << '\n';
    outIterations << width << " " << height << '\n';
    outIterations << graph.getMaxIterations() << '\n';

    //image body
    for (unsigned int pixY = 0; pixY < height; ++pixY) {
        if (pixY > 0) {
            outCoordinates << '\n';
            outEscape << '\n';
            outIterations << '\n';
        }
        for (unsigned int pixX = 0; pixX < width; ++pixX) {
            if (pixX > 0) {
                outCoordinates << " ";
                outEscape << " ";
                outIterations << " ";
            }
            outCoordinates << graph.getCoordinates(pixX, pixY);
            outEscape << graph.getEscape(pixX, pixY);
            outIterations << graph.getIterations(pixX, pixY);
        }
    }

    std::cout << "✅ Debug files written to ./debug_files/\n";
}

void ImageRenderer::printPPM(const MandelbrotGraph &graph) {
    // create folder
    fs::create_directories("Mandelbrot_Images");

    //create file
    std::ofstream out("Mandelbrot_Images/image.pgm");
    if (!out) throw std::runtime_error("Failed to open Mandelbrot_Images/image.ppm");

    std::cout << "📝 Writing file to ./Mandelbrot_Images/\n";

    //header variables
    const Complex center = graph.getCenter();
    const double zoom = graph.getZoom();
    const unsigned int width  = graph.getImageWidth();
    const unsigned int height = graph.getImageHeight();
    const unsigned int minIterations = graph.getMinIterations();
    const unsigned int maxIterations = graph.getMaxIterations();

    //header for outIterations
    out << "P2\n";
    out << " # Mandelbrot grayscale map\n";
    out << " # Center Coordinates: " << center << '\n';
    out << " # Zoom: " << zoom << '\n';
    out << width << " " << height << '\n';
    out << 255 << '\n';

    //precalc variables
    double ratio = 255.0 / (maxIterations - minIterations);

    //image body
    for (unsigned int pixY = 0; pixY < height; ++pixY) {
        if (pixY > 0) {
            out << '\n';
        }
        for (unsigned int pixX = 0; pixX < width; ++pixX) {
            if (pixX > 0) {
                out << " ";
            }
            unsigned int pixel = static_cast<unsigned int> ((graph.getIterations(pixX, pixY) - minIterations) * ratio);
            out << pixel;
        }
    }

    std::cout << "  ✅ File written to ./Mandelbrot_Images/\n";
}