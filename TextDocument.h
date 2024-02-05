#ifndef TEXT_DOCUMENT_H
#define TEXT_DOCUMENT_H

#define MAX_FILE_SIZE 65536

class TextDocument {

public:

    void initialize(const char* filename);
    int get_line_count();
    void get_line(int line_number, char* buffer, int buffer_size);

private:

    char file_buffer[MAX_FILE_SIZE];
    int file_buffer_size;
    int line_buffer[MAX_FILE_SIZE];
    int line_buffer_size;

    void initialize_line_buffer();
};

#endif

