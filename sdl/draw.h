#ifndef DRAW_H
#define DRAW_H

#include "app_state.h"

#define CELL_SIZE 10
#define CELL_PADDING 1
#define CELL_TOTAL_SIZE (CELL_SIZE + CELL_PADDING * 2)

void draw_board(AppState *app_state, uint64_t total_board_width, uint64_t total_board_height);

#endif
