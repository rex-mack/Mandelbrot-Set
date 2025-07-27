#include <iostream>

#include "mandelbrot.hpp"
#include "imageRenderer.hpp"



int main() {
    try {
        const unsigned int width   = 3000;    // Small for sanity
        const unsigned int height  = 1800;
        const unsigned int zoom    = 100;
        const unsigned int maxIter = 250;
        const Complex center(-0.75, 0.1);    // Classic seahorse valley

        // Step 1: Build graph
        MandelbrotGraph graph(width, height, center, zoom);
        graph.setIterationDepth(maxIter);

        // Step 2: Write debug files
        ImageRenderer image;
        image.printPPM(graph);

    } catch (const std::exception& e) {
        std::cerr << "🔥 Fatal error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}