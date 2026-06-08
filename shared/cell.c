#include "cell.h"


Cell *get_cell(Cell *cells, int x, int y, int width) {
  return &cells[y * width + x];
}

const Cell *get_cell_const(const Cell *cells, int x, int y, int width) {
  return &cells[y * width + x];
}
