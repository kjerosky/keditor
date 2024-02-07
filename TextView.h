#ifndef TEXT_VIEW_H
#define TEXT_VIEW_H

#include <cairo.h>

#include "TextDocument.h"

class TextView {

public:

    TextView(const char* filename);
    ~TextView();

    void draw_to_surface(cairo_surface_t* surface);
    void setup_font(cairo_t* cr);
    int get_current_line();
    int get_line_count();
    void shift_view(int line_count_delta);
    void go_to_line(int line_number);
    int get_line_height(cairo_surface_t* surface);

private:

    TextDocument* text_document;
    int current_line;
};

#endif

