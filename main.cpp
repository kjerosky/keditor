#include <gtk/gtk.h>

#include "TextView.h"

TextView* text_view;
cairo_surface_t* text_view_surface = NULL;

gboolean configure_event(GtkWidget* widget, GdkEventConfigure* event, gpointer data) {
    if (text_view_surface) {
        cairo_surface_destroy(text_view_surface);
    }

    text_view_surface = gdk_window_create_similar_surface(
        gtk_widget_get_window(widget),
        CAIRO_CONTENT_COLOR,
        gtk_widget_get_allocated_width(widget),
        gtk_widget_get_allocated_height(widget)
    );

    return TRUE;
}

gboolean draw(GtkWidget* widget, cairo_t* cr, gpointer data) {
    text_view->draw_to_surface(text_view_surface);

    cairo_set_source_surface(cr, text_view_surface, 0, 0);
    cairo_paint(cr);

    return FALSE;
}

void close_window() {
    if (text_view_surface) {
        cairo_surface_destroy(text_view_surface);
    }
}

void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "keditor");
    g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 8);

    GtkWidget* frame = gtk_frame_new(NULL);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_IN);
    gtk_container_add(GTK_CONTAINER(window), frame);

    GtkWidget* drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 320, 180);
    gtk_container_add(GTK_CONTAINER(frame), drawing_area);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw), NULL);
    g_signal_connect(drawing_area, "configure-event", G_CALLBACK(configure_event), NULL);

    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    text_view = new TextView("test.txt");

    GtkApplication* app = gtk_application_new("org.kjerosky.keditor", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    delete text_view;
    g_object_unref(app);
    return status;
}

