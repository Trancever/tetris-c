#include "cell_color.h"
#include "cell.h"

CellColorRGB cell_color_to_rgb(CellColor color) {
  switch (color) {
    case COLOR_CYAN:
      return (CellColorRGB){0, 255, 255};
    case COLOR_YELLOW:
      return (CellColorRGB){255, 255, 0};
    case COLOR_GREEN:
      return (CellColorRGB){0, 255, 0};
    case COLOR_RED:
      return (CellColorRGB){255, 0, 0};
    case COLOR_ORANGE:
      return (CellColorRGB){255, 165, 0};
    case COLOR_BLUE:
      return (CellColorRGB){0, 0, 255};
    case COLOR_PURPLE:
      return (CellColorRGB){128, 0, 128};
    case COLOR_NONE:
    default:
      return (CellColorRGB){0, 0, 0};
  }
}
