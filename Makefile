# 1. set compiler and flags
CC = gcc
# -Wall, -Wextra: enable warning messages
# -Iinclude: reference header files in the include folder
CFLAGS = -Wall -Wextra -Iinclude
# -lncurses: link ncurses library
# -lpthread: link thread library for system programming (pthread.h) [2, 3]
LDFLAGS = -lncursesw -lpthread

# 2. directory settings
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# 3. target executable name
TARGET = $(BIN_DIR)/gimbap

# 4. source and object file lists
# find all .c files in the src folder and convert them to .o file list
SRCS = $(shell find $(SRC_DIR) -name "*.c")
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# find all .h files in the include folder
HEADERS = $(shell find $(INC_DIR) -name "*.h")

# 5. default target
all: $(TARGET)

# 6. executable file creation rule
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 7. object file creation rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 8. utility rules
# delete generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run