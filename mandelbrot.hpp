#include "config.h"
#include "complex.cpp"

const unsigned int section_size = 16;

struct Section {
    Section *right;
    Section *down;
    Complex section[section_size][section_size];
    bool section_done;
};

class Mandelbrot_Graph {
    private:
    Section *head;
    unsigned int width;
    unsigned int height;
    int total_recursions;

    void new_left_section(); //easy
    void new_right_section(); //easy
    void new_top_section(); //hard
    void new_section_down(Section *insert_down_here); //medium

    public:
    Mandelbrot_Graph(unsigned int width, unsigned int height);
    ~Mandelbrot_Graph();

    void set_coordinates(double center, int zoom);
    void change_graph_dimentions(unsigned int length, unsigned int height);
    void move_center(int x_pixels, int i_pixels);
    void increase_recursions(unsigned int plus_recursions);
};