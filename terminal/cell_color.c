#include "cell_color.h"

const char *cell_color_code(CellColor color) {
  switch (color) {
    case COLOR_CYAN:
      return "\033[36m";
    case COLOR_YELLOW:
      return "\033[33m";
    case COLOR_GREEN:
      return "\033[32m";
    case COLOR_RED:
      return "\033[31m";
    case COLOR_ORANGE:
      return "\033[38;5;208m";
    case COLOR_BLUE:
      return "\033[34m";
    case COLOR_PURPLE:
      return "\033[35m";
    default:
      return "\033[0m";
  }
}
