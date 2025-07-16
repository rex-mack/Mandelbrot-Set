#include "mandelbrot.hpp"

Mandelbrot_Graph::Mandelbrot_Graph(unsigned int width, unsigned int height) {
    new_top_section();

    Section *ptr = head;
    for (int i = section_size; i < height; i += section_size) {
        new_section_down(ptr);
        ptr = ptr->down;
    }
}

void Mandelbrot_Graph::new_top_section() {
    Section *ptr;
    Section *new_section = ptr;
    for (int x = 0; x < width; x += section_size) {
        ptr = new Section;
        ptr = ptr->right;
    }
    if (head == NULL) return;
    ptr = new_section;
    Section *ptr2 = head;
    for (int x = 0; x < width; x += section_size) {
        ptr->down = ptr2;
        ptr = ptr->right;
        ptr2 = ptr2->right;
    }
    head = new_section;
}

void Mandelbrot_Graph::new_section_down(Section *insert_down_here) {
    Section *ptr;
    Section *new_section = ptr;
    for (int x = 0; x < width; x += section_size) {
        ptr = new Section;
        ptr = ptr->right;
    }
    Section *save = insert_down_here->down; // save section down
    ptr = insert_down_here;
    Section *ptr2 = new_section;
    for (int x = 0; x < width; x += section_size) {
        ptr->down = ptr2;
        ptr = ptr->right;
        ptr2 = ptr2->right;
    }
    if (save == NULL) return;
    ptr = save;
    ptr2 = new_section;
    for (int x = 0; x < width; x += section_size) {
        ptr->down = ptr2;
        ptr = ptr->right;
        ptr2 = ptr2->right;
    }
}

Mandelbrot_Graph::~Mandelbrot_Graph() {
    kill(head);
}

void kill(Section *curr) {
    if(curr->right != NULL) {
        kill(curr->right);
    }
    killDown(curr);
}

void killDown(Section *curr) {
    if (curr->down != NULL) {
        killDown(curr->down);
    }
    delete curr;
}

void Mandelbrot_Graph::set_coordinates(double center, int zoom) {

}

void Mandelbrot_Graph::change_graph_dimentions(unsigned int length, unsigned int height) {

}
void Mandelbrot_Graph::move_center(int x_pixels, int i_pixels){

}
void Mandelbrot_Graph::increase_recursions(unsigned int plus_recursions){

}


void Mandelbrot_Graph::new_left_section() {
    if (height == 0) return;
    Section *left_head = new Section;
    Section *ptr = left_head;
    for(int i = section_size; i < height; i += section_size) {
        Section *new_section = new Section;
        ptr->down = new_section;
        ptr = ptr->down;
    }

}
void Mandelbrot_Graph::new_right_section() {
    if (head == NULL) return;
    Section *ptr = new Section;
    Section *ptr2 = head;
    while (ptr2->right != NULL) {
        ptr2 = ptr2->right;
    }
    Section *prev;

    ptr->right = ptr2;

    for (int i = section_size; i < height; i += section_size) {
        prev = ptr;
        ptr = ptr->down;
        ptr2 = ptr2->down;
        ptr = new Section;
        prev->down = ptr;
        ptr->right = ptr2;
    }
}