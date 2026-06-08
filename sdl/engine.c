#include <SDL3/SDL_timer.h>
#include <stdint.h>

#include "engine.h"

uint64_t current_time_ms(void) { return (uint64_t)SDL_GetTicks(); }
