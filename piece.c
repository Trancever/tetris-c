#include <stdio.h>
#include <stdlib.h>

#include "cell.h"
#include "piece.h"

const Cell *get_piece_cell_const(const Piece *piece, int x, int y) {
  return get_cell_const(piece->cells, x, y, piece->width);
}

static Cell *get_piece_cell(Piece *piece, int x, int y) {
  return get_cell(piece->cells, x, y, piece->width);
}

static void set_piece_cell(Piece *piece, int x, int y, bool occupied, CellColor color) {
  Cell *cell = get_piece_cell(piece, x, y);

  cell->occupied = occupied;
  cell->color = occupied ? color : COLOR_NONE;
}

static Piece create_piece(int width, int height) {
  if (width < 1 || height < 1) {
    fprintf(stderr, "fatal: piece dimensions out of bounds: width=%d height=%d\n", width, height);
    abort();
  }

  if (width > PIECE_CELLS_TOTAL_WIDTH || height > PIECE_CELLS_TOTAL_HEIGHT) {
    fprintf(stderr, "fatal: piece cells out of bounds: width=%d height=%d\n", width, height);
    abort();
  }

  return (Piece){.width = width, .height = height};
}

static PieceType random_piece_type(void) {
  return (PieceType)(rand() % PIECE_COUNT);
}

static Piece create_i_piece(void) {
  Piece piece = create_piece(1, 4);

  CellColor color = COLOR_CYAN;

  set_piece_cell(&piece, 0, 0, true, color);
  set_piece_cell(&piece, 0, 1, true, color);
  set_piece_cell(&piece, 0, 2, true, color);
  set_piece_cell(&piece, 0, 3, true, color);

  return piece;
}

static Piece create_o_piece(void) {
  Piece piece = create_piece(2, 2);

  CellColor color = COLOR_YELLOW;

  set_piece_cell(&piece, 0, 0, true, color);
  set_piece_cell(&piece, 1, 0, true, color);
  set_piece_cell(&piece, 0, 1, true, color);
  set_piece_cell(&piece, 1, 1, true, color);

  return piece;
}

static Piece create_s_piece(void) {
  Piece piece = create_piece(3, 2);

  CellColor color = COLOR_GREEN;

  set_piece_cell(&piece, 0, 1, true, color);
  set_piece_cell(&piece, 1, 0, true, color);
  set_piece_cell(&piece, 1, 1, true, color);
  set_piece_cell(&piece, 2, 0, true, color);

  return piece;
}

static Piece create_z_piece(void) {
  Piece piece = create_piece(3, 2);

  CellColor color = COLOR_RED;

  set_piece_cell(&piece, 0, 0, true, color);
  set_piece_cell(&piece, 1, 0, true, color);
  set_piece_cell(&piece, 1, 1, true, color);
  set_piece_cell(&piece, 2, 1, true, color);

  return piece;
}

static Piece create_l_piece(void) {
  Piece piece = create_piece(2, 3);

  CellColor color = COLOR_ORANGE;

  set_piece_cell(&piece, 0, 0, true, color);
  set_piece_cell(&piece, 0, 1, true, color);
  set_piece_cell(&piece, 0, 2, true, color);
  set_piece_cell(&piece, 1, 2, true, color);

  return piece;
}

static Piece create_j_piece(void) {
  Piece piece = create_piece(2, 3);

  CellColor color = COLOR_BLUE;

  set_piece_cell(&piece, 0, 2, true, color);
  set_piece_cell(&piece, 1, 0, true, color);
  set_piece_cell(&piece, 1, 1, true, color);
  set_piece_cell(&piece, 1, 2, true, color);

  return piece;
}

static Piece create_t_piece(void) {
  Piece piece = create_piece(3, 2);

  CellColor color = COLOR_PURPLE;

  set_piece_cell(&piece, 0, 0, true, color);
  set_piece_cell(&piece, 1, 0, true, color);
  set_piece_cell(&piece, 1, 1, true, color);
  set_piece_cell(&piece, 2, 0, true, color);

  return piece;
}

Piece create_random_piece(void) {
  PieceType type = random_piece_type();

  switch (type) {
  case PIECE_O:
    return create_o_piece();
  case PIECE_I:
    return create_i_piece();
  case PIECE_S:
    return create_s_piece();
  case PIECE_Z:
    return create_z_piece();
  case PIECE_L:
    return create_l_piece();
  case PIECE_J:
    return create_j_piece();
  case PIECE_T:
    return create_t_piece();
  default:
    return create_o_piece();
  }
}

Piece create_rotated_piece(const Piece *piece) {
  int new_piece_width = piece->height;
  int new_piece_height = piece->width;

  Piece new_piece = create_piece(new_piece_width, new_piece_height);

  for (int opy = 0; opy < piece->height; opy++) {
    for (int opx = 0; opx < piece->width; opx++) {
      const Cell *old_piece_cell = get_piece_cell_const(piece, opx, opy);

      int new_x = piece->height - 1 - opy;
      int new_y = opx;

      Cell *new_piece_cell = get_piece_cell(&new_piece, new_x, new_y);

      *new_piece_cell = *old_piece_cell;
    }
  }

  return new_piece;
}
