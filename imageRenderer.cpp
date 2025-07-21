#include <filesystem>
#include "imageRenderer.hpp"

namespace fs = std::filesystem;

/*
 ----- Image Renderer -----
 -- Public --
*/


//add setprecision to the coordiantes 
void writeDebugFiles(const MandelbrotGraph &graph) {
    // create folder
    fs::create_directories("debug_images");

    //create files
    std::ofstream outCoordinates("debug_images/debug_coordinates.txt");
    if (!outCoordinates) throw std::runtime_error("Failed to open debug_coordinates.txt");
    std::ofstream outEscape("debug_images/debug_escape.txt");
    if (!outEscape) throw std::runtime_error("Failed to open debug_escape.txt");
    std::ofstream outIterations("debug_images/debug_iterations.pgm");
    if (!outIterations) throw std::runtime_error("Failed to open debug_iterations.pgm");

    std::cout << "📝 Writing debug files to ./debug_images/\n";

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

    //header for outEscape
    outEscape << "P2\n";
    outEscape << " # Debug escape map\n";
    outEscape << " # Center Coordinates: " << center << '\n';
    outEscape << " # Zoom: " << zoom << '\n';

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
            outEscape << graph.getEscapeCoordinates(pixX, pixY);
            outIterations << graph.getEscapeIteration(pixX, pixY);
        }
    }

    std::cout << "✅ Debug files written to ./debug_images/\n";
}