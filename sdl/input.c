#include "game.h"
#include "input.h"

InputStatus handle_app_event(AppState *app_state, const SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return INPUT_EXIT;
  }

  if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == 'q') {
    return INPUT_EXIT;
  }

  GameState *game = &app_state->game;

  if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == 't') {
    game_toggle_pause(game);
    return INPUT_RESET_TIMER;
  }

  if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == 'r') {
    game_restart(game);
    return INPUT_RESET_TIMER;
  }

  if (game->game_status != GAME_RUNNING) {
    return INPUT_CONTINUE;
  }

  if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == 'a') {
    move_piece_left(game);
  }

  if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == 'd') {
    move_piece_right(game);
  }

  if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == 's') {
    game_step_down(game);
  }

  if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == 'w') {
    rotate_piece(game);
  }

  return INPUT_CONTINUE;
}
