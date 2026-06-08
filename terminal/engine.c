#include <errno.h>
#include <stdint.h>
#include <time.h>

#include "engine.h"

void sleep_ms(long milliseconds) {
  struct timespec ts;
  ts.tv_sec = milliseconds / 1000;
  ts.tv_nsec = (milliseconds % 1000) * 1000000L;

  while (nanosleep(&ts, &ts) == -1 && errno == EINTR) {
  }
}

uint64_t current_time_ms(void) {
  struct timespec ts;

  if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
    return 0;
  }

  return ((uint64_t)ts.tv_sec * 1000) + ((uint64_t)ts.tv_nsec / 1000000);
}
