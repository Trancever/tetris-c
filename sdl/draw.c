#include <stdint.h>

#include "draw.h"
#include "cell_color.h"

void draw_board(AppState *app_state, uint64_t total_board_width,
                uint64_t total_board_height) {
  uint64_t start_x = 50;
  uint64_t start_y = 50;

  static SDL_FPoint line_points[5];
  line_points[0] = (SDL_FPoint){start_x, start_y};
  line_points[1] = (SDL_FPoint){start_x + total_board_width, start_y};
  line_points[2] =
      (SDL_FPoint){start_x + total_board_width, start_y + total_board_height};
  line_points[3] = (SDL_FPoint){start_x, start_y + total_board_height};
  line_points[4] = (SDL_FPoint){start_x, start_y};

  SDL_SetRenderDrawColor(app_state->renderer, 50, 50, 50,
                         SDL_ALPHA_OPAQUE); /* grey, full alpha */
  SDL_RenderClear(app_state->renderer);     /* start with a blank canvas. */

  SDL_SetRenderDrawColor(app_state->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_FRect board_rect = {start_x, start_y, total_board_width + 4,
                          total_board_height + 4};
  SDL_RenderRect(app_state->renderer, &board_rect);

  Board render_board = app_state->game.board;
  board_overlay_piece_for_render(&render_board, &app_state->game.current_piece,
                                 app_state->game.piece_x,
                                 app_state->game.piece_y);

  for (int y = 0; y < BOARD_VISIBLE_HEIGHT; y++) {
    for (int x = 0; x < BOARD_WIDTH; x++) {
      int board_y = y + BOARD_HIDDEN_HEIGHT;
      const Cell *cell = get_board_cell_const(&render_board, x, board_y);

      if (cell->occupied) {
        int cell_x = (int)start_x + (x * CELL_TOTAL_SIZE) + 3;
        int cell_y = (int)start_y + (y * CELL_TOTAL_SIZE) + 3;

        SDL_FRect cell_rect = {cell_x, cell_y, CELL_SIZE, CELL_SIZE};

        CellColorRGB rgb = cell_color_to_rgb(cell->color);

        SDL_SetRenderDrawColor(app_state->renderer, rgb.red, rgb.green,
                               rgb.blue, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(app_state->renderer, &cell_rect);
      }
    }
  }
}
