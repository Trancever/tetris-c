#include <stdio.h>
#include <string.h>

#include "draw.h"
#include "board.h"

static void move_cursor(int row, int col) { printf("\033[%d;%dH", row, col); }

void terminal_clear(void) {
  printf("\033[2J");
  printf("\033[H");
}

static void draw_horizontal_line(int top, int left, int width) {
  move_cursor(top, left);

  printf("%s", BOARD_BORDER_VERTICAL);

  for (int row = 0; row < width; row++) {
    printf(BOARD_BORDER_HORIZONTAL);
  }

  printf("%s", BOARD_BORDER_VERTICAL);
}

static void draw_border_with_inner_horizontal_line(int top, int left, int width) {
  move_cursor(top, left);

  printf("%s", BOARD_BORDER_VERTICAL);

  draw_horizontal_line(top, left + 1, width - 2);

  printf("%s", BOARD_BORDER_VERTICAL);
}

static void draw_empty_line(int top, int left, int width) {
  move_cursor(top, left);
  printf("%s", BOARD_BORDER_VERTICAL);

  move_cursor(top, left + width + 1);
  printf("%s", BOARD_BORDER_VERTICAL);
}

static void draw_tetris_line(int top, int left, int width) {
  move_cursor(top, left);
  printf("%s", BOARD_BORDER_VERTICAL);

  int tetris_sign_length = 10;
  int width_of_single_side = (width - tetris_sign_length) / 2;
  bool is_odd = width % 2 ? true : false;

  for (int y = 0; y < width_of_single_side; y++) {
    printf("=");
  }

  printf("  TETRIS  ");

  for (int y = 0; y < width_of_single_side; y++) {
    printf("=");
  }

  if (is_odd) {
    printf("=");
  }

  printf("%s", BOARD_BORDER_VERTICAL);
}

static void draw_border_with_board_line(int top, int left, const Board *board, int board_visible_y) {
    move_cursor(top, left);
    printf("|| ");

    for (int col = 0; col < BOARD_WIDTH; col++) {
      const Cell *cell = get_board_cell_const(board, col, board_visible_y);

      if (cell->occupied) {
        // prints color code for occupied cell, then actual cell character and then default color code to reset
        printf("%s%s\033[0m ", cell_color_code(cell->color), BOARD_CELL_FILLED);
      } else {
        printf("%s ", BOARD_CELL_EMPTY);
      }
    }

    printf("||");

}

int draw_board(int top, int left, const Board *board) {
  int total_height = BOARD_VISIBLE_HEIGHT + 7;
  int total_width = BOARD_WIDTH * BOARD_CELL_WIDTH + 3;

  for (int row = 0; row < total_height; row++) {
    int y = top + row;

    if (row == 0 || row == 6 + BOARD_VISIBLE_HEIGHT) {
      draw_horizontal_line(y, left, total_width);
    } else if (row == 1 || row == 3) {
      draw_empty_line(y, left, total_width);
    } else if (row == 2) {
      draw_tetris_line(y, left, total_width);
    } else if (row == 4 || row == 5 + BOARD_VISIBLE_HEIGHT) {
      draw_border_with_inner_horizontal_line(y, left, total_width);
    } else if (row >= 5 && row < 5 + BOARD_VISIBLE_HEIGHT) {
      draw_border_with_board_line(y, left, board, row - 5 + BOARD_HIDDEN_HEIGHT);
    }
  }

  return total_width;
}

static void draw_box(int top, int left, int width, const char *label, const char *value) {
  draw_horizontal_line(top, left, width);

  move_cursor(top + 1, left);
  printf("%s", BOARD_BORDER_VERTICAL);

  int score_sign_length = 5;
  int width_of_single_side = (width - score_sign_length) / 2;

  move_cursor(top + 1, left + width_of_single_side + 1);
  printf("%s", label);

  move_cursor(top + 1, left + (width_of_single_side * 2) + score_sign_length + 1);
  printf("%s", BOARD_BORDER_VERTICAL);

  draw_horizontal_line(top + 2, left, width);

  move_cursor(top + 3, left);
  printf("%s", BOARD_BORDER_VERTICAL);

  int score_value_length = 3;
  int score_width_of_single_side = (width - score_value_length) / 2;

  move_cursor(top + 3, left + score_width_of_single_side + 1);
  printf("%s", value);

  move_cursor(top + 3, left + (score_width_of_single_side * 2) + score_value_length + 1);
  printf("%s", BOARD_BORDER_VERTICAL);

  draw_horizontal_line(top + 4, left, width);
}

int draw_score(int top, int left, int score) {
  char score_as_string[4];
  snprintf(score_as_string, sizeof(score_as_string), "%03d", score);

  int width = 11;

  draw_box(top, left, width, "SCORE", score_as_string);

  return width;
}

int draw_level(int top, int left, int level) {
  char level_as_string[4];
  snprintf(level_as_string, sizeof(level_as_string), "%03d", level);

  int width = 11;

  draw_box(top, left, width, "LEVEL", level_as_string);

  return width;
}

static void draw_centered_text_line(int top, int left, int width, const char *text) {
  int text_length = (int)strlen(text);
  int left_padding = (width - text_length) / 2;
  int right_padding = width - text_length - left_padding;

  move_cursor(top, left);
  printf("%s", BOARD_BORDER_VERTICAL);

  for (int i = 0; i < left_padding; i++) {
    printf(" ");
  }

  printf("%s", text);

  for (int i = 0; i < right_padding; i++) {
    printf(" ");
  }

  printf("%s", BOARD_BORDER_VERTICAL);
}

static void draw_legend_row(int top, int left, const char *left_key,
                            const char *left_action, const char *right_key,
                            const char *right_action) {
  move_cursor(top, left);
  printf("%s %-3s %-15s %-3s %-23s%s", BOARD_BORDER_VERTICAL, left_key,
         left_action, right_key, right_action, BOARD_BORDER_VERTICAL);
}

int draw_legend(int top, int left) {
  int width = 48;

  draw_horizontal_line(top, left, width);
  draw_centered_text_line(top + 1, left, width, "CONTROLS");
  draw_horizontal_line(top + 2, left, width);

  draw_legend_row(top + 3, left, "A/D", "MOVE", "T", "PAUSE");
  draw_legend_row(top + 4, left, "W", "ROTATE", "R", "RESTART");
  draw_legend_row(top + 5, left, "S", "SOFT DROP", "Q", "QUIT");

  draw_horizontal_line(top + 6, left, width);

  return width;
}

int draw_next_piece(int top, int left, const Piece *piece) {
  int width = 14;
  char *label = "NEXT";

  draw_horizontal_line(top, left, width);

  move_cursor(top + 1, left);
  printf("%s", BOARD_BORDER_VERTICAL);

  int score_sign_length = 4;
  int width_of_single_side = (width - score_sign_length) / 2;

  move_cursor(top + 1, left + width_of_single_side + 1);
  printf("%s", label);

  move_cursor(top + 1, left + (width_of_single_side * 2) + score_sign_length + 1);
  printf("%s", BOARD_BORDER_VERTICAL);

  draw_horizontal_line(top + 2, left, width);

  int piece_width = piece->width;
  int piece_height = piece->height;

  int local_top = top + 3;
  int total_height = 6;
  int delta_height = total_height - piece_height;
  int single_side_empty_height = delta_height / 2;

  for (int y = 0; y < total_height; y++) {
    move_cursor(local_top + y, left);
    printf("%s", BOARD_BORDER_VERTICAL);


    int single_side_empty_width = (width - (piece_width * 2)) / 2;

    for (int x = 0; x < single_side_empty_width; x++) {
      printf(" ");
    }

    for (int x = 0; x < piece_width; x++) {
      if (y < single_side_empty_height || y >= single_side_empty_height + piece_height) {
        printf("  ");
        continue;
      }

      const Cell *cell = get_piece_cell_const(piece, x, y - single_side_empty_height);

      if (cell->occupied) {
        // prints color code for occupied cell, then actual cell character and then default color code to reset
        printf("%s%s\033[0m ", cell_color_code(cell->color), BOARD_CELL_FILLED);
      } else {
        printf("%s ", BOARD_CELL_EMPTY);
      }
    }

    for (int x = 0; x < single_side_empty_width; x++) {
      printf(" ");
    }
  
    printf("%s", BOARD_BORDER_VERTICAL);
  }

  draw_horizontal_line(local_top + total_height, left, width);

  return 0;
}
