#include "SDL3/SDL_stdinc.h"
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "app_state.h"
#include "board.h"
#include "draw.h"
#include "engine.h"
#include "game.h"
#include "game_timer.h"
#include "input.h"

static uint64_t total_board_width = CELL_TOTAL_SIZE * BOARD_WIDTH;
static uint64_t total_board_height = CELL_TOTAL_SIZE * BOARD_VISIBLE_HEIGHT;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  srand((unsigned int)time(NULL));

  AppState *app = SDL_calloc(1, sizeof(AppState));
  if (app == NULL) {
    return SDL_APP_FAILURE;
  }

  *appstate = app;

  SDL_SetAppMetadata("Example Renderer Lines", "1.0",
                     "com.example.renderer-lines");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    SDL_free(app);
    return SDL_APP_FAILURE;
  }

  int window_width = (int)total_board_width + 100;
  int window_height = (int)total_board_height + 100;

  if (!SDL_CreateWindowAndRenderer("examples/renderer/lines", window_width,
                                   window_height, SDL_WINDOW_RESIZABLE,
                                   &app->window, &app->renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    SDL_free(app);
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(app->renderer, window_width, window_height,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  app->game = initialize_game();
  game_timer_reset(&app->game_timer);
  app->last_time = current_time_ms();

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  AppState *app_state = appstate;
  InputStatus input_status = handle_app_event(app_state, event);

  if (input_status == INPUT_EXIT) {
    return SDL_APP_SUCCESS;
  }

  if (input_status == INPUT_RESET_TIMER) {
    game_timer_reset(&app_state->game_timer);
  }

  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  AppState *app_state = appstate;
  uint64_t current_time = current_time_ms();
  uint64_t elapsed_ms = current_time - app_state->last_time;
  app_state->last_time = current_time;

  if (app_state->game.game_status == GAME_RUNNING &&
      game_timer_update(&app_state->game_timer, &app_state->game, elapsed_ms)) {
    game_step_down(&app_state->game);
  }

  draw_board(app_state, total_board_width, total_board_height);

  SDL_RenderPresent(app_state->renderer); /* put it all on the screen! */

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  (void)result;

  SDL_free(appstate);
  /* SDL will clean up the window/renderer for us. */
}
