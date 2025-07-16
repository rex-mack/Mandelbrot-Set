#include "mandelbrot.hpp"

MandelbrotGraph::MandelbrotGraph() {
    head = nullptr;
    width = 0;
    height = 0;
    total_recursions = 0;
}

void free_grid(Section* curr) {
    while (curr != nullptr) {
        Section* nextCol = curr->right;
        free_column(curr);
        curr = nextCol;
    }
}

void free_column(Section* curr) {
    while (curr != nullptr) {
        Section* nextRow = curr->down;
        delete curr;
        curr = nextRow;
    }
}
MandelbrotGraph::~MandelbrotGraph() {
    free_grid(head);
}

void MandelbrotGraph::resizeImage(unsigned int setWidth, unsigned int setHeight) {
    if (setWidth > MAX_IMAGE_WIDTH) {
        cout << "ERROR: mandelbrot.cpp Tried to set image width > " << setWidth << endl;
        cout << "   Change in config.h" << endl;
        return;
    }
    if (setHeight > MAX_IMAGE_HEIGHT) {
        cout << "ERROR: mandelbrot.cpp Tried to set image height > " << setHeight << endl;
        cout << "   Change in config.h" << endl;
        return;
    }
    if ((setWidth == 0) || (setHeight == 0)) return;

    width = setWidth;
    height = setHeight;

    head = new_row();
    Section *prev = head;
    for(unsigned int i = section_size; i < height; i += section_size) {
        link_rows(prev, new_row());
        prev = prev->down;
    }
}

Section *MandelbrotGraph::new_row() {
    if (width == 0) return nullptr;
    Section *headSection = new Section;
    Section *curr = headSection;
    for(unsigned int i = section_size; i < width; i += section_size) {
        Section *newSection = new Section;
        curr->right = newSection;
        curr = curr->right;
    }
    return headSection;
}
void MandelbrotGraph::link_rows(Section *sectionHead, Section *newSectionHead) {
    if((sectionHead == nullptr) || (newSectionHead == nullptr)) return;

    Section *sectionDownHead = sectionHead->down;

    Section *top = sectionHead;
    Section *bottom = newSectionHead;
    for(unsigned int x = 0; x < width; x += section_size) {
        top->down = bottom;
        top = top->right;
        bottom = bottom->right;
    }

    if(sectionDownHead == nullptr) return;
    top = newSectionHead;
    bottom = sectionDownHead;
    for(unsigned int x = 0; x < width; x += section_size) {
        top->down = bottom;
        top = top->right;
        bottom = bottom->right;
    }
}

Section *MandelbrotGraph::new_column() {
    if (height == 0) return nullptr;
    Section *headSection = new Section;
    Section *curr = headSection;
    for(unsigned int i = section_size; i < height; i += section_size) {
        Section *newSection = new Section;
        curr->down = newSection;
        curr = curr->down;
    }
    return headSection;
}
void MandelbrotGraph::link_columns(Section *sectionHead, Section *new_sectionHead) {
    if((sectionHead == nullptr) || (new_sectionHead == nullptr)) return;

    Section *sectionRightHead = sectionHead->right;

    Section *left = sectionHead;
    Section *right = new_sectionHead;
    for(unsigned int i = 0; i < height; i += section_size) {
        left->right = right;
        left = left->down;
        right = right->down;
    }

    if(sectionRightHead == nullptr) return;
    left = new_sectionHead;
    right = sectionRightHead;
    for(unsigned int i = 0; i < height; i += section_size) {
        left->right = right;
        left = left->down;
        right = right->down;
    }
}