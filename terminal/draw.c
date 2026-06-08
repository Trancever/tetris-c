#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "draw.h"
#include "cell_color.h"

enum {
  BOARD_CELL_WIDTH = 2,
  BOARD_PANEL_INNER_WIDTH = BOARD_WIDTH * BOARD_CELL_WIDTH + 3,
  BOARD_PANEL_HEIGHT = BOARD_VISIBLE_HEIGHT + 7,
  BOARD_PLAYFIELD_TOP_ROW = 5,
  BOX_INNER_WIDTH = 11,
  LEGEND_INNER_WIDTH = 48,
  NEXT_PIECE_INNER_WIDTH = 14,
  NEXT_PIECE_CONTENT_HEIGHT = 6,
  NUMBER_TEXT_CAPACITY = 16
};

enum {
  LEGEND_KEY_WIDTH = 3,
  LEGEND_GROUP_GAP_WIDTH = 1
};

typedef struct {
  const char *left_key;
  const char *left_action;
  const char *right_key;
  const char *right_action;
} LegendRow;

static const LegendRow LEGEND_ROWS[] = {
    {"A/D", "MOVE", "T", "PAUSE"},
    {"W", "ROTATE", "R", "RESTART"},
    {"S", "SOFT DROP", "Q", "QUIT"},
};

static void move_cursor(int row, int col) { printf("\033[%d;%dH", row, col); }

static void write_repeat(char ch, int count) {
  for (int i = 0; i < count; i++) {
    putchar(ch);
  }
}

static int text_width(const char *text) {
  size_t length = strlen(text);

  if (length > (size_t)INT_MAX) {
    fprintf(stderr, "fatal: text too long to draw\n");
    abort();
  }

  return (int)length;
}

static void write_cell(const Cell *cell) {
  if (cell->occupied) {
    printf("%s■\033[0m ", cell_color_code(cell->color));
    return;
  }

  fputs("  ", stdout);
}

static void draw_horizontal_border(int top, int left, int width) {
  move_cursor(top, left);
  putchar('|');
  write_repeat('-', width);
  putchar('|');
}

static void draw_text_row(int top, int left, int width, const char *text) {
  int length = text_width(text);

  if (length > width) {
    length = width;
  }

  int left_padding = (width - length) / 2;
  int right_padding = width - length - left_padding;

  move_cursor(top, left);
  putchar('|');
  write_repeat(' ', left_padding);
  printf("%.*s", length, text);
  write_repeat(' ', right_padding);
  putchar('|');
}

static void draw_filled_title_row(int top, int left, int width,
                                  const char *title, char fill) {
  int length = text_width(title);

  if (length > width) {
    length = width;
  }

  int left_padding = (width - length) / 2;
  int right_padding = width - length - left_padding;

  move_cursor(top, left);
  putchar('|');
  write_repeat(fill, left_padding);
  printf("%.*s", length, title);
  write_repeat(fill, right_padding);
  putchar('|');
}

static void draw_empty_row(int top, int left, int width) {
  move_cursor(top, left);
  putchar('|');
  write_repeat(' ', width);
  putchar('|');
}

static void draw_board_separator(int top, int left) {
  move_cursor(top, left);
  fputs("||", stdout);
  write_repeat('-', BOARD_PANEL_INNER_WIDTH - 2);
  fputs("||", stdout);
}

static void draw_board_cells_row(int top, int left, const Board *board,
                                 int board_y) {
  move_cursor(top, left);
  fputs("|| ", stdout);

  for (int col = 0; col < BOARD_WIDTH; col++) {
    write_cell(get_board_cell_const(board, col, board_y));
  }

  fputs("||", stdout);
}

static void draw_number_box(int top, int left, const char *label,
                            uint64_t value) {
  char value_text[NUMBER_TEXT_CAPACITY];
  (void)snprintf(value_text, sizeof(value_text), "%03" PRIu64, value);

  draw_horizontal_border(top, left, BOX_INNER_WIDTH);
  draw_text_row(top + 1, left, BOX_INNER_WIDTH, label);
  draw_horizontal_border(top + 2, left, BOX_INNER_WIDTH);
  draw_text_row(top + 3, left, BOX_INNER_WIDTH, value_text);
  draw_horizontal_border(top + 4, left, BOX_INNER_WIDTH);
}

static void draw_legend_row(int top, int left, const LegendRow *row) {
  int left_action_width = 0;
  int right_action_width = 0;
  size_t row_count = sizeof(LEGEND_ROWS) / sizeof(LEGEND_ROWS[0]);

  for (size_t i = 0; i < row_count; i++) {
    int current_left_action_width = text_width(LEGEND_ROWS[i].left_action);
    int current_right_action_width = text_width(LEGEND_ROWS[i].right_action);

    if (current_left_action_width > left_action_width) {
      left_action_width = current_left_action_width;
    }

    if (current_right_action_width > right_action_width) {
      right_action_width = current_right_action_width;
    }
  }

  int left_group_width =
      LEGEND_KEY_WIDTH + LEGEND_GROUP_GAP_WIDTH + left_action_width;
  int right_group_width =
      LEGEND_KEY_WIDTH + LEGEND_GROUP_GAP_WIDTH + right_action_width;
  int remaining_width =
      LEGEND_INNER_WIDTH - left_group_width - right_group_width;
  int outer_padding = remaining_width / 3;
  int middle_padding = remaining_width - (outer_padding * 2);

  move_cursor(top, left);
  putchar('|');
  write_repeat(' ', outer_padding);
  printf("%-*s %-*s", LEGEND_KEY_WIDTH, row->left_key, left_action_width,
         row->left_action);
  write_repeat(' ', middle_padding);
  printf("%-*s %-*s", LEGEND_KEY_WIDTH, row->right_key, right_action_width,
         row->right_action);
  write_repeat(' ', outer_padding);
  putchar('|');
}

static void draw_piece_preview_row(int top, int left, int width,
                                   const Piece *piece, int preview_y) {
  int piece_width = piece->width * BOARD_CELL_WIDTH;
  int horizontal_padding = (width - piece_width) / 2;

  move_cursor(top, left);
  putchar('|');
  write_repeat(' ', horizontal_padding);

  if (preview_y < 0 || preview_y >= piece->height) {
    write_repeat(' ', piece_width);
  } else {
    for (int x = 0; x < piece->width; x++) {
      write_cell(get_piece_cell_const(piece, x, preview_y));
    }
  }

  write_repeat(' ', width - piece_width - horizontal_padding);
  putchar('|');
}

void terminal_clear(void) {
  fputs("\033[2J", stdout);
  fputs("\033[H", stdout);
}

int draw_board(int top, int left, const Board *board) {
  for (int row = 0; row < BOARD_PANEL_HEIGHT; row++) {
    int y = top + row;

    if (row == 0 || row == BOARD_PANEL_HEIGHT - 1) {
      draw_horizontal_border(y, left, BOARD_PANEL_INNER_WIDTH);
    } else if (row == 1 || row == 3) {
      draw_empty_row(y, left, BOARD_PANEL_INNER_WIDTH);
    } else if (row == 2) {
      draw_filled_title_row(y, left, BOARD_PANEL_INNER_WIDTH, "  TETRIS  ",
                            '=');
    } else if (row == 4 || row == BOARD_PANEL_HEIGHT - 2) {
      draw_board_separator(y, left);
    } else {
      int board_y = BOARD_HIDDEN_HEIGHT + row - BOARD_PLAYFIELD_TOP_ROW;
      draw_board_cells_row(y, left, board, board_y);
    }
  }

  return BOARD_PANEL_INNER_WIDTH;
}

int draw_score(int top, int left, uint64_t score) {
  draw_number_box(top, left, "SCORE", score);
  return BOX_INNER_WIDTH;
}

int draw_level(int top, int left, uint64_t level) {
  draw_number_box(top, left, "LEVEL", level);
  return BOX_INNER_WIDTH;
}

int draw_legend(int top, int left) {
  size_t row_count = sizeof(LEGEND_ROWS) / sizeof(LEGEND_ROWS[0]);

  draw_horizontal_border(top, left, LEGEND_INNER_WIDTH);
  draw_text_row(top + 1, left, LEGEND_INNER_WIDTH, "CONTROLS");
  draw_horizontal_border(top + 2, left, LEGEND_INNER_WIDTH);

  for (size_t i = 0; i < row_count; i++) {
    draw_legend_row(top + 3 + (int)i, left, &LEGEND_ROWS[i]);
  }

  draw_horizontal_border(top + 3 + (int)row_count, left, LEGEND_INNER_WIDTH);

  return LEGEND_INNER_WIDTH;
}

int draw_next_piece(int top, int left, const Piece *piece) {
  int vertical_padding = (NEXT_PIECE_CONTENT_HEIGHT - piece->height) / 2;

  draw_horizontal_border(top, left, NEXT_PIECE_INNER_WIDTH);
  draw_text_row(top + 1, left, NEXT_PIECE_INNER_WIDTH, "NEXT");
  draw_horizontal_border(top + 2, left, NEXT_PIECE_INNER_WIDTH);

  for (int row = 0; row < NEXT_PIECE_CONTENT_HEIGHT; row++) {
    draw_piece_preview_row(top + 3 + row, left, NEXT_PIECE_INNER_WIDTH, piece,
                           row - vertical_padding);
  }

  draw_horizontal_border(top + 3 + NEXT_PIECE_CONTENT_HEIGHT, left,
                         NEXT_PIECE_INNER_WIDTH);

  return NEXT_PIECE_INNER_WIDTH;
}

void draw_game(const GameState *game) {
  Board render_board = game->board;
  board_overlay_piece_for_render(&render_board, &game->current_piece, game->piece_x,
                      game->piece_y);

  int board_width = draw_board(1, 1, &render_board);

  int score_left = board_width + 10;
  int score_width = draw_score(1, score_left, game->score);

  int level_left = score_left + score_width + 2;
  int level_width = draw_level(1, level_left, game_get_level(game));

  int next_piece_left = level_left + level_width + 10;
  draw_next_piece(1, next_piece_left, &game->next_piece);
  draw_legend(12, score_left);
  printf("\n");
}
