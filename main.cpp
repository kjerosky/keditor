#include <gtk/gtk.h>

#include "TextDocument.h"

int main(int argc, char *argv[]) {
    TextDocument text_document;
    text_document.initialize("test.txt");

    int line_count = text_document.get_line_count();
    char buffer[256];
    for (int i = 0; i < line_count; i++) {
        text_document.get_line(i, buffer, 256);
        printf("%d : \"%s\"\n", i, buffer);
    }

    return 0;
}

