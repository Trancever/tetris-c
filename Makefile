CC ?= cc
CFLAGS ?= -std=c11
CPPFLAGS ?= -D_POSIX_C_SOURCE=200809L
LDFLAGS ?=
WARNING_FLAGS := -Wall -Wextra -Wpedantic -Wstrict-prototypes \
	-Wmissing-prototypes -Wshadow -Wconversion

BUILD_DIR := build
TARGET := $(BUILD_DIR)/tetris
SOURCES := main.c board.c draw.c engine.c piece.c game.c cell.c input.c
OBJECTS := $(SOURCES:%.c=$(BUILD_DIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WARNING_FLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
