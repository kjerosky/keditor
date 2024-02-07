#include <gtk/gtk.h>

#include "TextView.h"

TextView* text_view;
GtkWidget* drawing_area;
cairo_surface_t* text_view_surface = NULL;
GtkAdjustment* horizontal_scroll_adjustment = NULL;
GtkAdjustment* vertical_scroll_adjustment = NULL;

gboolean configure_event(GtkWidget* widget, GdkEventConfigure* event, gpointer data) {
    if (text_view_surface) {
        cairo_surface_destroy(text_view_surface);
    }

    int drawing_area_width = gtk_widget_get_allocated_width(widget);
    int drawing_area_height = gtk_widget_get_allocated_height(widget);

    text_view_surface = gdk_window_create_similar_surface(
        gtk_widget_get_window(widget),
        CAIRO_CONTENT_COLOR,
        drawing_area_width,
        drawing_area_height
    );

    int line_height = text_view->get_line_height(text_view_surface);
    int page_size = (double)drawing_area_height / line_height;

    gtk_adjustment_set_step_increment(vertical_scroll_adjustment, 1);
    gtk_adjustment_set_page_increment(vertical_scroll_adjustment, page_size);
    gtk_adjustment_set_page_size(vertical_scroll_adjustment, page_size);
    gtk_adjustment_set_lower(vertical_scroll_adjustment, 0);
    gtk_adjustment_set_upper(vertical_scroll_adjustment, text_view->get_line_count() - 1 + gtk_adjustment_get_page_size(vertical_scroll_adjustment));
    gtk_adjustment_set_value(vertical_scroll_adjustment, text_view->get_current_line());

    return TRUE;
}

gboolean draw(GtkWidget* widget, cairo_t* cr, gpointer data) {
    text_view->draw_to_surface(text_view_surface);

    cairo_set_source_surface(cr, text_view_surface, 0, 0);
    cairo_paint(cr);

    return FALSE;
}

gboolean key_press_event(GtkWidget* widget, GdkEventKey* event, gpointer data) {
    if (event->keyval == GDK_KEY_Up) {
        text_view->shift_view(-1);
        gtk_adjustment_set_value(vertical_scroll_adjustment, text_view->get_current_line());
        gtk_widget_queue_draw(drawing_area);
    } else if (event ->keyval == GDK_KEY_Down) {
        text_view->shift_view(1);
        gtk_adjustment_set_value(vertical_scroll_adjustment, text_view->get_current_line());
        gtk_widget_queue_draw(drawing_area);
    } else if (event->keyval == GDK_KEY_Escape) {
        gtk_window_close(GTK_WINDOW(widget));
    }

    return FALSE;
}

gboolean handle_scroll_event(GtkWidget* widget, GdkEventScroll event, gpointer user_data) {
    if (event.delta_y == 0.0) {
        return FALSE;
    }

    text_view->shift_view((int)event.delta_y);
    gtk_adjustment_set_value(vertical_scroll_adjustment, text_view->get_current_line());
    gtk_widget_queue_draw(drawing_area);

    return FALSE;
}

gboolean vertical_scrollbar_changed_value(GtkRange* self, GtkScrollType* scroll, gdouble value, gpointer user_data) {
    int line_number = value - (int)value < 0.5 ? (int)value : (int)(value + 1);
    text_view->go_to_line(line_number);
    gtk_adjustment_set_value(vertical_scroll_adjustment, text_view->get_current_line());
    gtk_widget_queue_draw(drawing_area);

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
    gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
    g_signal_connect(window, "key_press_event", G_CALLBACK(key_press_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);
    gtk_container_set_border_width(GTK_CONTAINER(window), 8);

    GtkWidget* scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_overlay_scrolling(GTK_SCROLLED_WINDOW(scrolled_window), false);
    vertical_scroll_adjustment = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
    gtk_widget_set_size_request(scrolled_window, 320, 180);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
    g_signal_connect(scrolled_window, "scroll-event", G_CALLBACK(handle_scroll_event), NULL);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), drawing_area);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(draw), NULL);
    g_signal_connect(drawing_area, "configure-event", G_CALLBACK(configure_event), NULL);

    gtk_widget_show_all(window);

    // If a ScrolledWindow's child does not implement the Scrollable interface,
    // then it's wrapped in a GtkViewport. We need to do the following to disable
    // the scrolling of that viewport by the ScrolledWindow, so that we can control
    // it more directly.
    GtkViewport* scrolled_window_viewport = GTK_VIEWPORT(gtk_bin_get_child(GTK_BIN(scrolled_window)));
    GtkAdjustment* zeroed_adjustment = gtk_adjustment_new(0, 0, 0, 0, 0, 0);
    gtk_scrollable_set_vadjustment(GTK_SCROLLABLE(scrolled_window_viewport), zeroed_adjustment);

    GtkWidget* scrolled_window_vertical_scrollbar = gtk_scrolled_window_get_vscrollbar(GTK_SCROLLED_WINDOW(scrolled_window));
    g_signal_connect(GTK_RANGE(scrolled_window_vertical_scrollbar), "change-value", G_CALLBACK(vertical_scrollbar_changed_value), NULL);
}

int main(int argc, char *argv[]) {
    text_view = new TextView("test2.txt");

    GtkApplication* app = gtk_application_new("org.kjerosky.keditor", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);

    delete text_view;
    g_object_unref(app);
    return status;
}

