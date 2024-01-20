CC = clang 
CFLAGS = -std=c99 -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -Wall
BUILD_DIR = build

OBJ_FILES = main.o http.o client.o cli.o protocolindependent.o uri.o

# Prepend $(BUILD_DIR)/ to each object file
OBJ_FILES := $(addprefix $(BUILD_DIR)/, $(OBJ_FILES))

all: $(BUILD_DIR) tinycurl

tinycurl: $(OBJ_FILES)
	$(CC) $^ -o $@

# Rule to compile source files into object files in the build directory
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/*.o
	rm -f tinycurl

