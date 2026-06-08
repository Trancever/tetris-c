#ifndef APP_STATE_H
#define APP_STATE_H

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "game.h"
#include "game_timer.h"

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  GameState game;
  GameTimer game_timer;
  uint64_t last_time;
} AppState;

#endif
