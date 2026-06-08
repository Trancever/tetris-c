#ifndef CELL_H
#define CELL_H

#include <stdbool.h>

typedef enum {
  COLOR_NONE = 0,
  COLOR_CYAN,
  COLOR_YELLOW,
  COLOR_GREEN,
  COLOR_RED,
  COLOR_ORANGE,
  COLOR_BLUE,
  COLOR_PURPLE
} CellColor;

typedef struct {
  bool occupied;
  CellColor color;
} Cell;

Cell *get_cell(Cell *cells, int x, int y, int width);

const Cell *get_cell_const(const Cell *cells, int x, int y, int width);

#endif
