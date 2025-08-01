#include <string>

#include "mandelbrot.hpp"
/*
Cout statements
*/

void printWelcomeMessage();
void printMenu();

/*
File Saving
*/

void saveImage(std::string filename = "Mandelbrot_Image");

bool executeCommand(std::string command, MandelbrotGraph &graph);

void openAtImage(std::string filename);

void clearDebugFiles();