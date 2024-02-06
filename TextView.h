#ifndef TEXT_VIEW_H
#define TEXT_VIEW_H

#include <cairo.h>

#include "TextDocument.h"

class TextView {

public:

    TextView(const char* filename);
    ~TextView();

    void draw_to_surface(cairo_surface_t* surface);

private:

    TextDocument* text_document;
};

#endif

