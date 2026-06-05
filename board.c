#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "cell.h"
#include "piece.h"

static Cell *get_board_cell(Board *board, int x, int y) {
  return get_cell(board->cells, x, y, BOARD_WIDTH);
}

const Cell *get_board_cell_const(const Board *board, int x, int y) {
  return get_cell_const(board->cells, x, y, BOARD_WIDTH);
}

static void require_board_bounds(int x, int y) {
  if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
    fprintf(stderr, "fatal: board cell out of bounds: x=%d y=%d\n", x, y);
    abort();
  }
}

bool board_lock_piece(Board *board, const Piece *piece, int piece_x, int piece_y) {
  bool touched_hidden_rows = false;

  for (int py = 0; py < piece->height; py++) {
    for (int px = 0; px < piece->width; px++) {
      const Cell *piece_cell =
          get_piece_cell_const(piece, px, piece->height - 1 - py);

      if (!piece_cell->occupied) {
        continue;
      }

      int board_x = piece_x + px;
      int board_y = piece_y - py;

      require_board_bounds(board_x, board_y);

      if (board_y < BOARD_HIDDEN_HEIGHT) {
        touched_hidden_rows = true;
      }

      Cell *board_cell = get_board_cell(board, board_x, board_y);

      *board_cell = *piece_cell;
    }
  }

  return touched_hidden_rows;
}

void board_overlay_piece_for_render(Board *board, const Piece *piece, int piece_x,
                         int piece_y) {
  for (int py = 0; py < piece->height; py++) {
    for (int px = 0; px < piece->width; px++) {
      const Cell *piece_cell =
          get_piece_cell_const(piece, px, piece->height - 1 - py);

      if (!piece_cell->occupied) {
        continue;
      }

      int board_x = piece_x + px;
      int board_y = piece_y - py;

      require_board_bounds(board_x, board_y);

      Cell *board_cell = get_board_cell(board, board_x, board_y);

      *board_cell = *piece_cell;
    }
  }
}

bool board_can_place_piece(const Board *board, const Piece *piece, int piece_x,
                           int piece_y) {
  for (int py = 0; py < piece->height; py++) {
    for (int px = 0; px < piece->width; px++) {
      const Cell *piece_cell = get_piece_cell_const(piece, px, py);

      if (!piece_cell->occupied) {
        continue;
      }

      int board_x = piece_x + px;
      int board_y = piece_y - (piece->height - 1 - py);

      if (board_x < 0 || board_x >= BOARD_WIDTH || board_y < 0 || board_y >= BOARD_HEIGHT) {
        return false;
      }

      if (get_board_cell_const(board, board_x, board_y)->occupied) {
        return false;
      }
    }
  }

  return true;
}

bool board_can_move_piece_down(const Board *board, const Piece *piece,
                               int piece_x, int piece_y) {
  return board_can_place_piece(board, piece, piece_x, piece_y + 1);
}

bool board_can_move_piece_left(const Board *board, const Piece *piece,
                               int piece_x, int piece_y) {
  return board_can_place_piece(board, piece, piece_x - 1, piece_y);
}

bool board_can_move_piece_right(const Board *board, const Piece *piece,
                                int piece_x, int piece_y) {
  return board_can_place_piece(board, piece, piece_x + 1, piece_y);
}

static bool board_row_completed(const Board *board, int row) {
  for (int col = 0; col < BOARD_WIDTH; col++) {
    if (!get_board_cell_const(board, col, row)->occupied) {
      return false;
    }
  }

  return true;
}

static void board_shift_rows_down_from(Board *board, int row) {
  for (int shift_row = row; shift_row >= 0; shift_row--) {
    for (int col = 0; col < BOARD_WIDTH; col++) {
      Cell *cell = get_board_cell(board, col, shift_row);

      if (shift_row == 0) {
        *cell = (Cell){0};
      } else {
        *cell = *get_board_cell(board, col, shift_row - 1);
      }
    }
  }
}

int board_clear_completed_rows(Board *board) {
  int rows_cleared = 0;
  int row = BOARD_VISIBLE_HEIGHT - 1;

  while (row >= 0) {
    int visible_row = row + BOARD_HIDDEN_HEIGHT;

    if (!board_row_completed(board, visible_row)) {
      row--;
      continue;
    }

    rows_cleared++;
    board_shift_rows_down_from(board, visible_row);
  }

  return rows_cleared;
}
