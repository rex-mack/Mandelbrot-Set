#include <csignal>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "config.h"
#include "Code/complex.hpp"
#include "Code/mandelbrot.hpp"

/*
Handeling Signals
*/

void handleSignal(int signal) {// Handle signals like Ctrl+C
    if (signal == SIGINT) {
        std::cout << "\nExiting gracefully...\n";
        exit(0);
    }
}

/*
Menue Messages
*/

void printWelcomeMessage() {
    std::cout << " ----- Mandelbrot Set Renderer -----\n";
    std::cout << "This program renders the Mandelbrot Set and saves it as an image.\n";
    std::cout << "Press Ctrl+C to exit at any time.\n";
    std::cout << " -------------------------------------\n\n";
}

void printMenu() {
    std::cout << " --------------------------- Command Menu ---------------------------\n";
    std::cout << " - 'set center <x> <y>'        - to set the center of the Image to coordinates <x> <y>i\n";
    std::cout << " - 'set zoom <factor>'         - to set the zoom level of the Image\n";
    std::cout << " - 'set iterations <number>'   - to set the maximum number of iterations for the Mandelbrot Set\n";
    std::cout << " - 'move <x> <y>'              - to move the center of the Image by <x> <y> pixels\n";
    std::cout << " - 'zoom <factor>'             - to change the zoom level\n";
    std::cout << " - 'increase depth <factor>'   - to increase the maximum number of iterations\n";
    std::cout << " - 'decrease depth <factor>'   - to decrease the maximum number of iterations\n";
    std::cout << " - 'print'                     - to print the current image called \"Mandelbrot_Image.pgm\"\n";
    std::cout << " - 'change image size <x> <y>' - to change the dimentions of the image\n";
    std::cout << " - 'save [file name]'          - to save the current image. If [file name] is omitted, the default name 'Mandelbrot_Image' will be used.\n";
    std::cout << " - 'exit'                      - to exit the program\n";
    std::cout << " - 'menu'                      - to show this menu again\n";
    std::cout << " --------------------------------------------------------------------\n";
}



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

    std::cout << " ------------------------------------\n";
}

/*
Image Processing
*/
void saveImage(std::string filename = "Mandelbrot_Image") {
    std::cout << "  --- Saving Mandelbrot Set Image ---\n";

    // Create the directory if it doesn't exist
    std::filesystem::create_directories("Mandelbrot_Images");

    // Construct the full file path
    std::string filePath = "Mandelbrot_Images/" + filename + ".pgm";
    int fileCounter = 0;
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
    std::cout << " ------------------------------------\n";
}

/*
Command Processing
*/

void parseString(const std::string &input, int &argc, std::vector<std::string> &argv) {
    std::istringstream stream(input);
    std::string arg;

    argc = 0; // Reset argument count
    argv.clear(); // Clear previous arguments
    while (stream >> arg) {
        argv.push_back(arg);
        argc++;
    }
}

bool executeCommand(std::string command, MandelbrotGraph &graph) {
    int argc; // Argument count
    std::vector<std::string> argv; // Arguments vector
    parseString(command, argc, argv);

    if (argc == 0) {
        return true; // No command entered
    }
    if (argc == 4 && argv[0] == "set" && argv[1] == "center") {
        try {
            double x = std::stod(argv[2]);
            double i = std::stod(argv[3]);
            double radius = IMAGE_RADIUS_NUMERATOR/IMAGE_RADIUS_DENOMINATOR;

            // Check if the coordinates are within the valid range
            graph.setGraphValues(Complex(x, i), graph.getZoom(), graph.getTargetIterations(), graph.getImageWidth(), graph.getImageHeight());
            std::cout << '\n';
            printImage(graph);
        } catch (const std::invalid_argument &e) { // Handle invalid input
            std::cerr << "Invalid center coordinates: " << argv[2] << " - " << argv[3] << '\n';
        } catch (const std::out_of_range &e) { // Handle out of range input
            std::cerr << "Center coordinates out of range: " << argv[2] << " - " << argv[3] << '\n';
        }
        return true;
    }
    if (argc == 3 && argv[0] == "set" && argv[1] == "zoom") {
        try {
            double zoom = std::stod(argv[2]);
            graph.setGraphValues(graph.getCenter(), zoom, graph.getTargetIterations(), graph.getImageWidth(), graph.getImageHeight());
            std::cout << '\n';
            printImage(graph);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid zoom factor: " << argv[2] << '\n';
        }
        return true;
    }
    if (argc == 3 && argv[0] == "set" && argv[1] == "iterations") {
        try {
            unsigned int iterations = std::stoul(argv[2]);
            graph.setGraphValues(graph.getCenter(), graph.getZoom(), iterations, graph.getImageWidth(), graph.getImageHeight());
            std::cout << '\n';
            printImage(graph);
        } catch (const std::invalid_argument &e) {
            std::cerr << "ERROR set iterations: " << e.what() << '\n';
        }
        return true;
    }
    if (argc == 3 && argv[0] == "move") {
        try {
            int x = std::stoi(argv[1]);
            int y = std::stoi(argv[2]);
            graph.moveCenterByPixel(x, y);
            std::cout << '\n';
            printImage(graph);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid pixel values: " << argv[1] << " - " << argv[2] << '\n';
        }
        return true;
    }
    if (argc == 2 && argv[0] == "zoom") {
        try {
            double factor = std::stod(argv[1]);
            graph.zoomIn(factor);
            std::cout << '\n';
            printImage(graph);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid zoom factor: " << argv[1] << '\n';
        }
        return true;
    }
    if (argc == 3 && argv[0] == "increase" && argv[1] == "depth") {
        try {
            unsigned int factor = std::stoul(argv[2]);
            graph.increaseIterations(factor);
            std::cout << '\n';
            printImage(graph);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid factor for increasing iterations: " << argv[2] << '\n';
        }
        return true;
    }
    if (argc == 3 && argv[0] == "decrease" && argv[1] == "depth") {
        try {
            unsigned int factor = std::stoul(argv[2]);
            if (factor > graph.getTargetIterations()) {
                std::cerr << "Cannot decrease iterations below current target iterations: " << graph.getTargetIterations() << '\n';
                return true; // Continue the loop
            }
            graph.increaseIterations(-factor); // Decrease iterations by negative factor
            std::cout << '\n';
            printImage(graph);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid factor for decreasing iterations: " << argv[2] << '\n';
        }
        return true;
    }
    if (argc == 1 && argv[0] == "print") {
        printImage(graph);
        return true; // Continue the loop
    }
    if (argc == 5 && argv[0] == "change" && argv[1] == "image" && argv[2] == "size") {
        try {
            unsigned int width = std::stoul(argv[3]);
            unsigned int height = std::stoul(argv[4]);
            graph.setGraphValues(graph.getCenter(), graph.getZoom(), graph.getTargetIterations(), width, height);
            std::cout << '\n';
            printImage(graph);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Invalid image size: " << argv[3] << " - " << argv[4] << '\n';
        }
        return true;
    }
    if (argc == 1 && argv[0] == "save") {
        try {
            saveImage();
        } catch (const std::exception &e) {
            std::cerr << "Error saving image: " << e.what() << '\n';
        }
        return true; // Continue the loop
    }
    if (argc == 2 && argv[0] == "save") {
        try {
            saveImage(argv[1]);
        } catch (const std::exception &e) {
            std::cerr << "Error saving image: " << e.what() << '\n';
        }
        return true; // Continue the loop
    }
    if (argc == 1 && argv[0] == "exit") {
        std::cout << "Exiting the program...\n";
        return false; // Exit the program
    }
    if (argc == 1 && argv[0] == "menu") {
        printMenu();
        return true; // Continue the loop
    }
    if(argc == 1 && argv[0] == "debug") {
        try {
            graph.writeDebugFiles();
        } catch (const std::exception &e) {
            std::cerr << "Error writing debug files: " << e.what() << '\n';
        }
        return true; // Continue the loop
    }

    // Handle other commands
    std::cout << "Unknown command: " << command << '\n';
    std::cout << "Type 'menu' to see the list of available commands.\n";
    return true; // Continue the loop
}

void clearDebugFiles() {
    if(!std::filesystem::exists("debug_files")) {
        return; // No debug files to clear
    }
    std::cout << "  --- Clearing Debug Files ---\n";
    std::filesystem::remove_all("debug_files");
    std::filesystem::create_directory("debug_files");
    std::cout << "  ✅ Debug files cleared.\n";
    std::cout << " -------------------------------\n\n";
}

int main () {
    std::signal(SIGINT, handleSignal); // Handle Ctrl+C to exit gracefully

    std::cout << '\n'; // leave a line space after input

    clearDebugFiles(); // Clear debug files at the start

    printWelcomeMessage(); // Print welcome message

    // Create the Graph object
    MandelbrotGraph mandelbrotGraph;

    std::cout << '\n';

    // Render the image
    try {
        printImage(mandelbrotGraph);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    std::cout << '\n';

    // Print the menu
    printMenu();

    // main loop
    while (true) {
        std::cout << "\nEnter command: ";
        std::string command;
        std::getline(std::cin, command);
        std::cout << '\n'; // leave a line space after input

        if (executeCommand(command, mandelbrotGraph) == false) return 0; // Exit the program
    }
}