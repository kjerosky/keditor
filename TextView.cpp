#include "TextView.h"

TextView::TextView(const char* filename) {
    text_document = new TextDocument();
    text_document->initialize(filename);
}

TextView::~TextView() {
    delete text_document;
}

void TextView::draw_to_surface(cairo_surface_t* surface) {
    cairo_t* cr = cairo_create(surface);

    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 20);

    cairo_font_extents_t font_extents;
    cairo_font_extents(cr, &font_extents);
    int line_height = font_extents.height;

    cairo_set_source_rgb(cr, 0.12, 0.12, 0.12);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);

    int y = -1;
    for (int i = 0; i < text_document->get_line_count(); i++) {
        char line[256];
        text_document->get_line(i, line, 256);

        y += line_height;
        cairo_move_to(cr, 0, y);
        cairo_show_text(cr, line);
    }

    cairo_destroy(cr);
}

