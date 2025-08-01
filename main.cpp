#include <csignal>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "config.h"
#include "Code/complex.hpp"
#include "Code/imageRenderer.hpp"
#include "Code/mandelbrot.hpp"
#include "Code/menu.hpp"
#include "Code/parseString.hpp"

/*
Handeling Signals
*/

void handleSignal(int signal) {// Handle signals like Ctrl+C
    if (signal == SIGINT) {
        std::cout << "\nExiting gracefully...\n";
        exit(0);
    }
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