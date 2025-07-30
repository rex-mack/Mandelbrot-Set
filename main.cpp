#include <csignal>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "imageRenderer.hpp"

void handleSignal(int signal) { // Handle signals like Ctrl+C
    if (signal == SIGINT) {
        std::cout << "\nExiting gracefully...\n";
        exit(0);
    }
}

void validateGraphParameters(const MandelbrotGraph &graph) {
    if (graph.getImageWidth() == 0 || graph.getImageHeight() == 0) {
        throw std::invalid_argument("Image dimensions cannot be zero");
    }
    if (graph.getMaxIterations() == 0) {
        throw std::invalid_argument("Maximum iterations must be greater than zero");
    }
    if (graph.getCenter().getReal() < -2.0 || graph.getCenter().getReal() > 2.0 ||
        graph.getCenter().getImag() < -2.0 || graph.getCenter().getImag() > 2.0) {
        throw std::invalid_argument("Center coordinates must be within the range [-2, 2]");
    }
}

void printImage(const MandelbrotGraph &graph) {
    std::cout << " --- Rendering Mandelbrot Set Image -----\n";

    //check for errors
    validateGraphParameters(graph);

    //create .ppm file
    std::ofstream out("Mandelbrot_Image.pgm");
    if (!out) throw std::runtime_error("Failed to open Mandelbrot_Image.pgm");

    std::cout << "📝 Writing file to ./Mandelbrot_Image.pgm\n";

    //header variables
    const Complex center = graph.getCenter();
    const double zoom = graph.getZoom();
    const unsigned int width  = graph.getImageWidth();
    const unsigned int height = graph.getImageHeight();
    const unsigned int maxIterations = graph.getMaxIterations();

    //Write the PPM header
    out << "P2\n"; // P2 format for grayscale
    out << " # Mandelbrot grayscale map\n";
    out << " # Center Coordinates: " << center << '\n';
    out << " # Zoom: " << zoom << '\n';
    out << width << " " << height << '\n';
    out << 255 << '\n';

    // Write the pixel data
    for (unsigned int pixY = 0; pixY < height; ++pixY) {
        if (pixY > 0) {
            out << '\n';
        }
        for (unsigned int pixX = 0; pixX < width; ++pixX) {
            if (pixX > 0) {
                out << " ";
            }
            unsigned int iterations = graph.getIterations(pixX, pixY);
            unsigned int colorValue = (iterations * 255) / maxIterations; // Normalize to 0-255
            out << colorValue;
        }
    }

    std::cout << "  ✅ Image written to ./Mandelbrot_Image.pgm\n";
    out.close();

    std::cout << " ------------------------------------\n\n";
}

void saveImage(string filename = "Mandelbrot_Image") {
    std::cout << "  --- Saving Mandelbrot Set Image ---\n";

    // Create the directory if it doesn't exist
    std::filesystem::create_directories("Mandelbrot_Images");

    // Construct the full file path
    std::string filePath = "Mandelbrot_Images/" + filename + ".pgm";
    int fileCounter = 1;
    while (std::filesystem::exists(filePath)) {
        filePath = "Mandelbrot_Images/" + filename + "(" + std::to_string(fileCounter++) + ").pgm";
    }

    // Copy the image file to the new location
    std::filesystem::copy("Mandelbrot_Image.pgm", filePath);
    if(!std::filesystem::exists(filePath)) {
        throw std::runtime_error("Failed to save image: " + filePath);
    }

    // Print success message
    std::cout << "  ✅ Image saved to " << filePath << '\n';
    std::cout << " ------------------------------------\n\n";
}


int main () {
    std::signal(SIGINT, handleSignal); // Handle Ctrl+C to exit gracefully

    std::cout << " ----- Mandelbrot Set Renderer -----\n";
    std::cout << "This program renders the Mandelbrot Set and saves it as an image.\n";
    std::cout << "Press Ctrl+C to exit at any time.\n";
    std::cout << " -------------------------------------\n\n";

    // Create the Graph object
    MandelbrotGraph mandelbrotGraph(1920, 1080, Complex(0.0, 0.0), 1.0);

    // Set the iteration depth
    mandelbrotGraph.setIterationDepth(50);

    // Render the image
    try {
        printImage(mandelbrotGraph);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    // Print the menu
    printMenu();

    // main loop
    while (true) {
        std::cout << "Enter command: ";
        std::string command;
        std::getline(std::cin, command);

        if (command.empty()) { // Check for empty command
            continue;
        }
    }
}

void printMenu() {
    std::cout << " -- Command Menu -----\n";
    std::cout << " - 'set center <x> <y>'       - to set the center of the Image to coordinates <x> <y>i\n";
    std::cout << " - 'set zoom <factor>'        - to set the zoom level of the Image\n";
    std::cout << " - 'set iterations <number>'  - to set the maximum number of iterations for the Mandelbrot Set\n";
    std::cout << " - 'move <x> <y>'             - to move the center of the Image by <x> <y> pixels\n";
    std::cout << " - 'zoom <factor>'            - to change the zoom level\n";
    std::cout << " - 'increase depth <factor>'  - to increase the maximum number of iterations\n";
    std::cout << " - 'decrease depth <factor>'  - to decrease the maximum number of iterations\n";
    std::cout << " - 'print'                    - to print the current image\n";
    std::cout << " - 'save [file name]'         - to save the current image. If [file name] is omitted, the default name 'Mandelbrot_Image' will be used.\n";
    std::cout << " - 'exit'                     - to exit the program\n";
    std::cout << " - 'menu'                     - to show this menu again\n";
    std::cout << " ---------------------\n\n";
}