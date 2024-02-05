#include "TextDocument.h"

#include <fstream>
#include <string.h>
#include <algorithm>

void TextDocument::initialize(const char* filename) {
    std::ifstream file;
    file.open(filename, std::ios_base::in | std::ios_base::binary);
    file.read(file_buffer, MAX_FILE_SIZE);
    file_buffer_size = file.gcount();
    file.close();

    initialize_line_buffer();
}

void TextDocument::initialize_line_buffer() {
    int line_start_index = 0;
    line_buffer_size = 0;

    for (int i = 0; i < file_buffer_size; i++) {
        if (file_buffer[i] == '\n') {
            line_buffer[line_buffer_size++] = line_start_index;
            line_start_index = i + 1;
        }
    }
}

int TextDocument::get_line_count() {
    return line_buffer_size;
}

void TextDocument::get_line(int line_number, char* buffer, int buffer_size) {
    int line_start_index = line_buffer[line_number];
    int line_end_index = line_number + 1 >= get_line_count() ? file_buffer_size : line_buffer[line_number + 1];
    int num_characters = std::min(line_end_index - line_start_index, buffer_size - 1);

    strncpy(buffer, (char *)&(file_buffer[line_start_index]), num_characters);
    buffer[num_characters] = '\0';
}

