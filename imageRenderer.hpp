#include <string>
#include "mandelbrot.hpp"

using namespace std;

class ImageRenderer {
    private:
    MandelbrotGraph &graph;
    
    public:
        static void writePPM(const MandelbrotGraph& graph, const string &filepath);
};