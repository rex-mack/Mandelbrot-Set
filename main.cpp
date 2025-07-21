#include <iostream>

#include "mandelbrot.hpp"
#include "imageRenderer.hpp"


// Your writeDebugFiles() function prototype
void writeDebugFiles(const MandelbrotGraph &graph);

int main() {
    try {
        const unsigned int width   = 200;    // Small for sanity
        const unsigned int height  = 150;
        const unsigned int zoom    = 100;
        const unsigned int maxIter = 250;
        const Complex center(-0.75, 0.1);    // Classic seahorse valley

        // Step 1: Build graph
        MandelbrotGraph graph(width, height, center, zoom);
        graph.setIterationDepth(maxIter);

        // Step 2: Write debug files
        writeDebugFiles(graph);

    } catch (const std::exception& e) {
        std::cerr << "🔥 Fatal error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}