#include "config.h"
#include "complex.cpp"

const unsigned int section_size = 16;

struct Section {
    Section *right;
    Section *down;
    Complex section[section_size][section_size];
    bool section_done;
};

class MandelbrotGraph {
    private:
    Section *head;
    unsigned int width;
    unsigned int height;
    unsigned int total_recursions;

    Section *new_row();
    void link_rows(Section *section_head, Section *new_section_head);
    Section *new_column();
    void link_columns(Section *section_head, Section *new_section_head);

    public:
    MandelbrotGraph();
    ~MandelbrotGraph();

    void resize_image(unsigned int set_width, unsigned int set_height);
    void set_coordinates(double center, int zoom);
};