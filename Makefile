OUTPUT_FILE = keditor

SOURCE_FILES = \
	main.cpp \
	TextDocument.cpp

COMPILE_FLAGS = `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0`



all:
	g++ $(COMPILE_FLAGS) -o $(OUTPUT_FILE) $(SOURCE_FILES)

clean:
	rm -rf $(OUTPUT_FILE)

