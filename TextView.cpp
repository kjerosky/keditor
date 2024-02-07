#include "TextView.h"

#include <algorithm>

TextView::TextView(const char* filename) {
    text_document = new TextDocument();
    text_document->initialize(filename);

    current_line = 0;
}

TextView::~TextView() {
    delete text_document;
}

void TextView::draw_to_surface(cairo_surface_t* surface) {
    cairo_t* cr = cairo_create(surface);
    setup_font(cr);

    int line_height = get_line_height(surface);

    cairo_set_source_rgb(cr, 0.12, 0.12, 0.12);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);

    int y = -1;
    for (int i = current_line; i < text_document->get_line_count(); i++) {
        char line[256];
        text_document->get_line(i, line, 256);

        y += line_height;
        cairo_move_to(cr, 0, y);
        cairo_show_text(cr, line);
    }

    cairo_destroy(cr);
}

void TextView::setup_font(cairo_t* cr) {
    cairo_select_font_face(cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 20);
}

int TextView::get_current_line() {
    return current_line;
}

int TextView::get_line_count() {
    return text_document->get_line_count();
}

void TextView::shift_view(int line_count_delta) {
    // gross clamping
    current_line += line_count_delta;
    current_line = std::max(0, current_line);
    current_line = std::min(current_line, get_line_count() - 1);
}

void TextView::go_to_line(int line_number) {
    // gross clamping
    current_line = line_number;
    current_line = std::max(0, current_line);
    current_line = std::min(current_line, get_line_count() - 1);
}

int TextView::get_line_height(cairo_surface_t* surface) {
    cairo_t* cr = cairo_create(surface);
    setup_font(cr);

    cairo_font_extents_t font_extents;
    cairo_font_extents(cr, &font_extents);
    int line_height = font_extents.height;

    cairo_destroy(cr);

    return line_height;
}

