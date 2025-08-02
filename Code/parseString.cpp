#include <sstream>

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