CC ?= cc
CFLAGS ?= -std=c11
CPPFLAGS ?= -D_POSIX_C_SOURCE=200809L
LDFLAGS ?=
WARNING_FLAGS := -Wall -Wextra -Wpedantic -Wstrict-prototypes \
	-Wmissing-prototypes -Wshadow -Wconversion

BUILD_DIR := build
TUI_TARGET := $(BUILD_DIR)/tetris
SDL_TARGET := $(BUILD_DIR)/tetris-sdl

SDL_VENDOR_DIR := vendor/SDL
SDL_BUILD_DIR := $(BUILD_DIR)/vendor/SDL
SDL_INSTALL_DIR := $(abspath $(BUILD_DIR)/vendor/sdl-install)
SDL_INCLUDE_DIR := $(SDL_INSTALL_DIR)/include
SDL_LIB_DIR := $(SDL_INSTALL_DIR)/lib
SDL_STAMP := $(SDL_BUILD_DIR)/.installed
SDL_CMAKE_FLAGS ?= -DCMAKE_BUILD_TYPE=RelWithDebInfo \
	-DCMAKE_INSTALL_PREFIX=$(SDL_INSTALL_DIR) \
	-DCMAKE_INSTALL_LIBDIR=lib \
	-DSDL_SHARED=ON \
	-DSDL_STATIC=OFF \
	-DSDL_TEST_LIBRARY=OFF \
	-DSDL_TESTS=OFF \
	-DSDL_EXAMPLES=OFF
SDL_CPPFLAGS := -I$(SDL_INCLUDE_DIR)
SDL_LDFLAGS := -Wl,-rpath,$(SDL_LIB_DIR)
SDL_LDLIBS := -L$(SDL_LIB_DIR) -lSDL3

SHARED_SOURCES := shared/board.c shared/cell.c shared/game.c shared/game_timer.c shared/piece.c
TUI_SOURCES := main.c terminal/draw.c terminal/input.c terminal/engine.c terminal/cell_color.c
SDL_SOURCES := main_sdl.c sdl/engine.c sdl/cell_color.c sdl/input.c sdl/draw.c

SHARED_OBJECTS := $(SHARED_SOURCES:%.c=$(BUILD_DIR)/%.o)
TUI_UI_OBJECTS := $(TUI_SOURCES:%.c=$(BUILD_DIR)/%.o)
SDL_UI_OBJECTS := $(SDL_SOURCES:%.c=$(BUILD_DIR)/%.o)

TUI_OBJECTS := $(SHARED_OBJECTS) $(TUI_UI_OBJECTS)
SDL_OBJECTS := $(SHARED_OBJECTS) $(SDL_UI_OBJECTS)

SHARED_CPPFLAGS := -Ishared
TUI_CPPFLAGS := -Iterminal
SDL_UI_CPPFLAGS := -Isdl $(SDL_CPPFLAGS)

.PHONY: all clean run run-sdl sdl vendor-sdl

all: $(TUI_TARGET)

$(TUI_TARGET): $(TUI_OBJECTS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(TUI_OBJECTS) -o $@

sdl: $(SDL_TARGET)

$(SDL_TARGET): $(SDL_STAMP) $(SDL_OBJECTS) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(SDL_LDFLAGS) $(SDL_OBJECTS) $(SDL_LDLIBS) -o $@

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WARNING_FLAGS) -c $< -o $@

$(SHARED_OBJECTS): CPPFLAGS += $(SHARED_CPPFLAGS)
$(TUI_UI_OBJECTS): CPPFLAGS += $(SHARED_CPPFLAGS) $(TUI_CPPFLAGS)
$(SDL_UI_OBJECTS): CPPFLAGS += $(SHARED_CPPFLAGS) $(SDL_UI_CPPFLAGS)
$(SDL_UI_OBJECTS): $(SDL_STAMP)

$(SDL_STAMP): | $(BUILD_DIR)
	@missing=0; \
	if ! test -f "$(SDL_VENDOR_DIR)/CMakeLists.txt"; then \
		echo "vendor/SDL was not found. Add SDL3 source at vendor/SDL before running make sdl."; \
		missing=1; \
	fi; \
	if ! command -v cmake >/dev/null 2>&1; then \
		echo "CMake was not found. Install CMake to build vendored SDL3."; \
		missing=1; \
	fi; \
	test $$missing -eq 0
	cmake -S "$(SDL_VENDOR_DIR)" -B "$(SDL_BUILD_DIR)" $(SDL_CMAKE_FLAGS)
	cmake --build "$(SDL_BUILD_DIR)"
	cmake --install "$(SDL_BUILD_DIR)"
	touch "$@"

vendor-sdl: $(SDL_STAMP)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TUI_TARGET)
	./$(TUI_TARGET)

run-sdl: $(SDL_TARGET)
	./$(SDL_TARGET)

clean:
	rm -rf $(BUILD_DIR)
