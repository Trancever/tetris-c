#ifndef CELL_COLOR_H
#define CELL_COLOR_H

#include <stdint.h>

#include "cell.h"

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} CellColorRGB;

CellColorRGB cell_color_to_rgb(CellColor color);

#endif
