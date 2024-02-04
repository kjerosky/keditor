OUTPUT_FILE = keditor
SOURCE_FILES = main.c
COMPILE_FLAGS = `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0`

all:
	gcc $(COMPILE_FLAGS) -o $(OUTPUT_FILE) $(SOURCE_FILES)

clean:
	rm -rf $(OUTPUT_FILE)

