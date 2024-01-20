CC = clang
CFLAGS = -std=c99 -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -Wall
BUILD_DIR = build
SRC_DIR = src

# List all source files in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Create a list of object files by replacing '.c' with '.o' in source files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

all: $(BUILD_DIR) tinycurl

tinycurl: $(OBJ_FILES)
	$(CC) $^ -o $(BUILD_DIR)/$@

# Rule to compile source files into object files in the build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)/* 
