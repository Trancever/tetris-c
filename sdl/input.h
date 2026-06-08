#ifndef INPUT_H
#define INPUT_H

#include "SDL3/SDL_events.h"
#include "app_state.h"

typedef enum {
  INPUT_CONTINUE = 0,
  INPUT_EXIT,
  INPUT_RESET_TIMER,
} InputStatus;

InputStatus handle_app_event(AppState *app_state, const SDL_Event *event);

#endif
