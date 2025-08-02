#include <iostream>
#include <filesystem>
#include <fstream>

#include "menu.hpp"
#include "parseString.hpp"
#include "imageRenderer.hpp"
#include "config.h"

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
    std::cout << " - 'menu'                      - to show this menu again\n";
    std::cout << " - 'set image <file name>'     - to set the image to a saved image in 'Mandelbrot_Images'\n";
    std::cout << " - 'set center <x> <y>'        - to set the center of the Image to coordinates <x> <y>i\n";
    std::cout << " - 'set zoom <factor>'         - to set the zoom level of the Image\n";
    std::cout << " - 'set iterations <number>'   - to set the maximum number of iterations for the Mandelbrot Set\n";
    std::cout << " - 'set image size <x> <y>' - to change the dimentions of the image\n";
    std::cout << " - 'move <x> <y>'              - to move the center of the Image by <x> <y> pixels\n";
    std::cout << " - 'zoom <factor>'             - to change the zoom level\n";
    std::cout << " - 'increase iterations  <factor>'   - to increase the maximum number of iterations\n";
    std::cout << " - 'decrease iterations  <factor>'   - to decrease the maximum number of iterations\n";
    std::cout << " - 'save [file name]'          - to save the current image. Default file name is 'Mandelbrot_Image'\n";
    std::cout << " - 'exit'                      - to exit the program\n";
    std::cout << " --------------------------------------------------------------------\n";
}

/*
Image Stuff
*/

void saveImage(std::string filename) {
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

void setImage(std::string filename, MandelbrotGraph &graph) {
    std::string filePath = "Mandelbrot_Images/" + filename + ".pgm";

    std::ifstream file(filePath);
    if (!file) throw std::runtime_error("Failed to open image file: " + filePath);

    int argc; // Argument count
    std::vector<std::string> argv; // Arguments vector
    std::cout << "  --- Setting Image to " << filePath << " ---\n";
    std::string line;

    std::getline(file, line); // Read the first line (P2)
    if (line != "P2") throw std::runtime_error("Invalid image format. Expected P2 format.");

    std::getline(file, line); // skip
    std::getline(file, line); // Read the comment line
    parseString(line, argc, argv);
    if (argc != 6) throw std::runtime_error("Invalid image header. Expected 6 arguments in the header.");
    Complex center(
        std::stod(argv[3]), // x coordinate
        std::stod(argv[5])  // i coordinate
    );

    std::getline(file, line); // Read the zoom line
    parseString(line, argc, argv);
    if (argc != 3) throw std::runtime_error("Invalid zoom line. Expected 3 arguments.");
    double zoom = std::stod(argv[2]);

    getline(file, line); // Read the target iterations line
    parseString(line, argc, argv);
    if (argc != 4) throw std::runtime_error("Invalid target iterations line. Expected 4 arguments.");
    unsigned int targetIterations = std::stoul(argv[3]);

    getline(file, line); // skip
    getline(file, line); // Read the image dimensions line
    parseString(line, argc, argv);
    if (argc != 2) throw std::runtime_error("Invalid image dimensions line. Expected 2 arguments.");
    unsigned int imageWidth = std::stoul(argv[0]);
    unsigned int imageHeight = std::stoul(argv[1]);
    std::cout << "Setting image to (" << center << ") with zoom: " << zoom << " and target iterations: " << targetIterations << '\n';
    std::cout << "  Image dimensions: " << imageWidth << "x" << imageHeight << "\n\n";
    graph.setGraphValues(center, zoom, targetIterations, imageWidth, imageHeight);
    std::cout << '\n';
    std::cout << "✅ Image set to " << filePath << "\n";
    std::cout << " ------------------------------------\n\n";
    file.close();

    printImage(graph); // Render the image after setting it
}

bool executeCommand(std::string command, MandelbrotGraph &graph) {
    int argc; // Argument count
    std::vector<std::string> argv; // Arguments vector
    parseString(command, argc, argv);

    if (argc == 0) {
        return true; // No command entered
    }
    if (argc == 1 && argv[0] == "menu") {
        printMenu();
        return true; // Continue the loop
    }
    if(argc == 3 && argv[0] == "set" && argv[1] == "image") {
        try {
            setImage(argv[2], graph);
            std::cout << '\n';
            printImage(graph);
        } catch (const std::exception &e) {
            std::cerr << "Error setting image: " << e.what() << '\n';
        }
        return true; // Continue the loop
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
            std::cerr << "Error setting center: " << e.what() << '\n';
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
            std::cerr << "Error setting zoom: " << argv[2] << '\n';
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
    if (argc == 5 && argv[0] == "set" && argv[1] == "image" && argv[2] == "size") {
        try {
            unsigned int width = std::stoul(argv[3]);
            unsigned int height = std::stoul(argv[4]);
            graph.setGraphValues(graph.getCenter(), graph.getZoom(), graph.getTargetIterations(), width, height);
            std::cout << '\n';
            printImage(graph);
        } catch (const std::invalid_argument &e) {
            std::cerr << "Error setting image size: " << e.what() << '\n';
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
            std::cerr << "Error zooming: " << e.what() << '\n';
        }
        return true;
    }
    if (argc == 3 && argv[0] == "increase" && argv[1] == "iterations ") {
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
    if (argc == 3 && argv[0] == "decrease" && argv[1] == "iterations ") {
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

void openAtImage(std::string filename) {
    std::cout << "  --- Opening Image in default viewer ---\n";
    std::string filepath = "Mandelbrot_Images/" + filename + ".pgm";

    std::ifstream file(filepath);
    if (!file) throw std::runtime_error("Failed to open: " + filepath);

    std::string line;
    std::getline(file, line); // Read the first line (P2)
}

void clearDebugFiles() {
    if(!std::filesystem::exists("debug_files")) {
        return; // No debug files to clear
    }
    std::cout << "  --- Clearing Debug Files ---\n";
    std::filesystem::remove_all("debug_files");
    std::cout << "  ✅ Debug files cleared.\n";
    std::cout << " -------------------------------\n\n";
}